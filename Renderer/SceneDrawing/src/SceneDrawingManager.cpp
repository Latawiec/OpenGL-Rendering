#include "../SceneDrawing/SceneDrawingManager.hpp"

#include "../SceneDrawing/Frustum.hpp"

#include <glad/glad.h>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace SceneDrawing {

ShadowMappingPass::SharedData SceneDrawingManager::createFittingShadowmapTransform(const Scene::Base::DirectionalLight& light, const glm::mat4 lightTransform, const Scene::Base::Camera& camera, const glm::mat4 cameraTransform) {

    const glm::mat4 lightWorldTransform = lightTransform;
    const glm::vec3 lightDirection = glm::normalize(lightWorldTransform * light.GetLightDirection());

    // We'll build new transform matrix for the light.
    const glm::vec3 cameraPosition = cameraTransform * glm::vec4(0, 0, 0, 1);
    const glm::vec3 cameraWorldRight = cameraTransform * camera.GetCameraOrientation() * glm::vec4(1, 0, 0, 0);
    const glm::vec3 cameraWorldUp =    cameraTransform * camera.GetCameraOrientation() * glm::vec4(0, 1, 0, 0);
    const glm::vec3 cameraWorldFront = cameraTransform * camera.GetCameraOrientation() * glm::vec4(0, 0, -1, 0);

    // I want to build perfect transform of shadowmap view - meaning side of shadowmap texture is parallel with the frustum.
    // For building the "perfect" space for shadowmap, we need to figure which 2 vectors we can use. Best is I think: two least parallel with light direction.
    const float rightDot = glm::abs(glm::dot(cameraWorldRight, lightDirection));
    const float upDot = glm::abs(glm::dot(cameraWorldRight, lightDirection));
    const float frontDot = glm::abs(glm::dot(cameraWorldRight, lightDirection));

    glm::vec3 firstSelected;
    glm::vec3 secondSelected;

    if (rightDot > upDot) {
        firstSelected = cameraWorldRight;
        if (upDot > frontDot) {
            secondSelected = cameraWorldUp;
        } else {
            secondSelected = cameraWorldFront;
        }
    } else {
        firstSelected = cameraWorldUp;
        if (upDot > frontDot) {
            secondSelected = cameraWorldUp;
        } else {
            secondSelected = cameraWorldFront;
        }
    }

    // Now we need to build a transform to make our selected vectors X and Y axis of light-view matrix.
    // I'll use glm::lookAt implementation partially.
    glm::mat4 lightNewTransform {1};
    {
        glm::vec3 f = glm::normalize(lightDirection);
        glm::vec3 u = glm::normalize(firstSelected);
        glm::vec3 s = glm::normalize(glm::cross(f, u));
        u = glm::cross(s, f);

        lightNewTransform[0][0] = s.x;
        lightNewTransform[1][0] = s.y;
        lightNewTransform[2][0] = s.z;
        lightNewTransform[0][1] = u.x;
        lightNewTransform[1][1] = u.y;
        lightNewTransform[2][1] = u.z;
        lightNewTransform[0][2] =-f.x;
        lightNewTransform[1][2] =-f.y;
        lightNewTransform[2][2] =-f.z;
        lightNewTransform[3][0] =-glm::dot(s, cameraPosition);
        lightNewTransform[3][1] =-glm::dot(u, cameraPosition);
        lightNewTransform[3][2] = glm::dot(f, cameraPosition);
    }

    Frustum frustum(camera); // this is frustum created in camera-space
    frustum.Transform(cameraTransform * camera.GetCameraOrientation()); // now it's in world-space.
    frustum.Transform(lightNewTransform);
    
    // Now we simply find min's and max'es from the frustum vertices in light space, and we done!
    const auto& frustumVertices = frustum.GetVertices();
    glm::vec3 minValues = frustumVertices[0];
    glm::vec3 maxValues = frustumVertices[0];

    for (int i=1; i<frustumVertices.size(); ++i) {
        const auto vertex = frustumVertices[i];
        minValues.x = vertex.x < minValues.x ? vertex.x : minValues.x;
        minValues.y = vertex.y < minValues.y ? vertex.y : minValues.y;
        minValues.z = vertex.z < minValues.z ? vertex.z : minValues.z;

        maxValues.x = vertex.x > maxValues.x ? vertex.x : maxValues.x;
        maxValues.y = vertex.y > maxValues.y ? vertex.y : maxValues.y;
        maxValues.z = vertex.z > maxValues.z ? vertex.z : maxValues.z;
    }

    const glm::mat4 lightProjectionTransform = glm::ortho(minValues.x, maxValues.x, minValues.y, maxValues.y, -maxValues.z, -minValues.z);
    
    ShadowMappingPass::SharedData viewData;
    viewData.lightViewTransform = lightNewTransform;
    viewData.lightProjectionTransform = lightProjectionTransform;

    return viewData;
}

SceneDrawingManager::SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight)
: _scene(scene)
, _transformProcessor(_scene)
, _deferredBuffers(windowWidth, windowHeight)
, _width(windowWidth)
, _height(windowHeight) {
    auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
    _framebufferPlane = Scene::Base::VertexData<Scene::Base::Layout::Sequential, Scene::Base::Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));
}

void SceneDrawingManager::Draw() {
    _transformProcessor.Update();
    ShadowMappingPass();
    BasePass();
    LightingPass();

    // {
    //     glDisable(GL_DEPTH_TEST);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     glViewport(0, 0, _width, _height);

    //     _textureDrawProgram.draw(_deferredBuffers.getTexture(GraphicBuffer::Output::Albedo));

    //     // Test Shadowmap
    //     glViewport(0, 0, _deferredBuffers.GetHeight()/3.f, _deferredBuffers.GetHeight()/3.f);
    //     const auto& shadowMap = *_directionalLightShadowMaps.begin();
    //     _textureDrawProgram.draw(shadowMap.second.getTexture());

    // }
}

void SceneDrawingManager::LightingPass() 
{
    LightingPass::SharedData data;
    data.positionTexture = _deferredBuffers.getTexture(GraphicBuffer::Output::Position);
    data.normalMapTexture = _deferredBuffers.getTexture(GraphicBuffer::Output::Normals);

    LightingPass::LightingPipelineManager::PropertiesSet properties = 0;

    for (const auto& sceneLight : _scene.GetSceneLights()) {
        const auto& nodeId = sceneLight.nodeId;
        const auto& lightNode = _scene.GetNode(sceneLight.nodeId);

        if (sceneLight.directionalLightId != Scene::Base::DirectionalLight::INVALID_ID) {
            const auto& lightId = sceneLight.directionalLightId;
            const auto& lightObject = _scene.GetDirectionalLight(lightId);

            properties |= LightingPass::LightingPipelineManager::PipelineProperties::LIGHTTYPE_DIRECTIONAL;

            const glm::mat4 lightTransform = _transformProcessor.GetNodeTransforms().at(nodeId);
            const glm::vec4 lightDirection = lightTransform * lightObject.GetLightDirection();
            data.directionalLightsTransforms.push_back(_directionalLightTransforms.at(lightId));
            data.directionalLightsDirections.push_back(lightDirection);
            data.directionalLightsShadowmapTextureIds.push_back(_directionalLightShadowMaps.at(lightId).getTexture());
        }
    }

    const auto& pipeline = _lightingPassPipelineManager.GetPipeline(properties);
    const auto binding = pipeline.Bind();
    pipeline.prepareShared(data);
    pipeline.prepareIndividual();

    // Just print to window for now.
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
    glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
}

void SceneDrawingManager::ShadowMappingPass()
{
    static const float TemporaryShadowMapSize = 1024;

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    for (const auto& sceneLight : _scene.GetSceneLights()) {
        const auto& nodeId = sceneLight.nodeId;
        const auto& lightNode = _scene.GetNode(sceneLight.nodeId);


        if (sceneLight.directionalLightId != Scene::Base::DirectionalLight::INVALID_ID) {
            const auto& lightId = sceneLight.directionalLightId;
            const auto& lightObject = _scene.GetDirectionalLight(lightId);

            if (!_directionalLightShadowMaps.contains(lightId)) {
                _directionalLightShadowMaps.emplace(lightId, DepthBuffer(TemporaryShadowMapSize, TemporaryShadowMapSize));
            }
            const auto& shadowMapBuffer = _directionalLightShadowMaps.at(lightId);
            const auto framebufferBinding = shadowMapBuffer.Bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            // TODO: Test... I'll need to either render it for each camera or introduce "Active Camera"...
            const auto& [cameraNodeId, cameraId] = *_scene.GetSceneViews().begin();
            const auto& camera = _scene.GetCamera(cameraId);
            const auto& cameraTransform = _transformProcessor.GetNodeTransforms().at(cameraNodeId);

            ShadowMappingPass::SharedData viewData = createFittingShadowmapTransform(lightObject, _transformProcessor.GetNodeTransforms().at(nodeId), camera, cameraTransform);
            // Remember it for drawing shadows
            _directionalLightTransforms[lightId] = viewData.lightProjectionTransform * viewData.lightViewTransform;

            for (const auto& sceneElement : _scene.GetSceneObjects()) {
                // Can't draw without geometry.
                if (sceneElement.meshId == Renderer::Scene::Base::Mesh::INVALID_ID) { continue; }
                // Can't draw without material for now. Maybe I'll replace it with a dummy material later on.
                if (sceneElement.materialId == Renderer::Scene::Base::Material::INVALID_ID) { continue; }

                const auto& material = _scene.GetMaterial(sceneElement.materialId);
                // It doesn't cast shadow... so who cares!
                if (!material.isCastingShadow()) { continue; }

                ShadowMappingPass::ShadowMappingPipelineManager::PropertiesSet properties = ShadowMappingPass::ShadowMappingPipelineManager::PipelineProperties::LIGHTTYPE_DIRECTIONAL;
                {
                    if (sceneElement.skinId != Renderer::Scene::Base::Skin::INVALID_ID) {
                        properties |= ShadowMappingPass::ShadowMappingPipelineManager::PipelineProperties::SKIN;
                    }
                }

                ShadowMappingPass::IndividualData objectData;
                objectData.objectModelTransform = _transformProcessor.GetNodeTransforms().at(sceneElement.nodeId);
                if (sceneElement.skinId != Renderer::Scene::Base::Skin::INVALID_ID) {
                    prepareSkin(sceneElement.skinId);
                    objectData.jointsArray = &_jointTransforms[sceneElement.skinId];
                }

                const auto& pipeline = _shadowMappingPassPipelineManager.GetPipeline(properties);
                const auto pipelineBinding = pipeline.Bind();
                pipeline.prepareShared(viewData);
                pipeline.prepareIndividual(objectData);

                const auto& mesh = _scene.GetMesh(sceneElement.meshId);
                Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { mesh.getVertexData() };

                glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
            }            
        }
    }

    glCullFace(GL_BACK);
}

void SceneDrawingManager::BasePass()
{
    // Get any camera for now...
    const auto& [nodeId, cameraId] = *_scene.GetSceneViews().begin();
    const auto& camera = _scene.GetCamera(cameraId);
    const auto viewTransform = glm::inverse(_transformProcessor.GetNodeTransforms().at(nodeId) * camera.GetCameraOrientation());
    const auto projTransform = camera.GetProjectionTransform();

    // If I was smart enough I'd also set it up in OpenGL here once. Maybe someday...
    BasePass::SharedData viewData;
    viewData.cameraProjectionTransform = projTransform;
    viewData.cameraViewTransform = viewTransform;

    const auto binding = _deferredBuffers.Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& sceneElement : _scene.GetSceneObjects()) {

        if (sceneElement.meshId == Renderer::Scene::Base::Mesh::INVALID_ID) {
            // Can't draw without geometry.
            continue;
        }

        if (sceneElement.materialId == Renderer::Scene::Base::Material::INVALID_ID) {
            // Can't draw without material for now. Maybe I'll replace it with a dummy material later on.
            continue;
        }

        // We'll for now set up properties for each draw call... Unfortunate!
        BasePass::BasePassPipelineManager::PropertiesSet properties = 0;
        {
            if (sceneElement.skinId != Renderer::Scene::Base::Skin::INVALID_ID) {
                properties |= BasePass::BasePassPipelineManager::PipelineProperties::SKIN;
            }

            const auto& material = _scene.GetMaterial(sceneElement.materialId);
            if (material.getTexture<Renderer::Scene::Base::Material::ETexture::Albedo>() != Renderer::Scene::Base::Texture::INVALID_ID) {
                properties |= BasePass::BasePassPipelineManager::PipelineProperties::BASE_COLOR_TEXTURE;
            }

            if (material.getTexture<Renderer::Scene::Base::Material::ETexture::Normal>() != Renderer::Scene::Base::Texture::INVALID_ID) {
                properties |= BasePass::BasePassPipelineManager::PipelineProperties::NORMAL_MAP_TEXTURE;
            }
        }

        BasePass::IndividualData objectData;
        objectData.meshId = sceneElement.nodeId; // Temporary
        objectData.objectModelTransform = _transformProcessor.GetNodeTransforms().at(sceneElement.nodeId);

        if (sceneElement.skinId != Renderer::Scene::Base::Skin::INVALID_ID) {
            prepareSkin(sceneElement.skinId);
            objectData.jointsArray = &_jointTransforms[sceneElement.skinId];
        }
        const auto& material = _scene.GetMaterial(sceneElement.materialId);
        if (material.getTexture<Renderer::Scene::Base::Material::ETexture::Albedo>() != Renderer::Scene::Base::Texture::INVALID_ID) {
            objectData.baseColorTexture = &_scene.GetTexture(material.getTexture<Renderer::Scene::Base::Material::ETexture::Albedo>());
        }
        if (material.getTexture<Renderer::Scene::Base::Material::ETexture::Normal>() != Renderer::Scene::Base::Texture::INVALID_ID) {
            objectData.normalMapTexture = &_scene.GetTexture(material.getTexture<Renderer::Scene::Base::Material::ETexture::Normal>());
        }

        const auto& pipeline = _basePassPipelineManager.GetPipeline(properties);
        const auto pipelineBinding = pipeline.Bind();
        pipeline.prepareShared(viewData);
        pipeline.prepareIndividual(objectData);

        const auto& mesh = _scene.GetMesh(sceneElement.meshId);
        Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { mesh.getVertexData() };

        glDrawElements(GL_TRIANGLES, mesh.getVertexData().vertexCount(), GL_UNSIGNED_SHORT, 0);
    }
}

void setTransforms(
    const glm::mat4& parentTransform,
    std::array<glm::mat4, 32>& jointTransforms,
    const Renderer::Scene::Scene& scene,
    const Renderer::Scene::Base::Skin& skin,
    const Renderer::Scene::Base::Skin::BoneLink& boneLink)
{
    const auto boneIndex = boneLink.boneIndex;
    const Renderer::Scene::Base::Skin::Bone& bone = skin.GetBones().at(boneIndex);
    const Renderer::Scene::Base::Node& node = scene.GetNode(bone.nodeId);

    const glm::mat4 transform = parentTransform * node.GetTransform() * bone.inverseBindMatrix;

    jointTransforms[boneIndex] = transform;

    for (const auto& childBoneLink : boneLink.childBoneLinks) {
        setTransforms(parentTransform * node.GetTransform(), jointTransforms, scene, skin, childBoneLink);
    }
}

void SceneDrawingManager::prepareSkin(const Renderer::Scene::Base::Skin::IdType& skinId) {
    const auto& skin = _scene.GetSkin(skinId);
    const auto& skinHierarchy = skin.GetBonesHierarchy();
    if (!_jointTransforms.contains(skinId))
    {
        _jointTransforms[skinId] = {};
    }
    auto& skinJointTransforms = _jointTransforms[skinId];
    skinJointTransforms.fill(glm::mat4(1));
    setTransforms(glm::mat4{1}, skinJointTransforms, _scene, skin, skinHierarchy);
}

} // namespace SceneDrawing
} // namespace Renderer
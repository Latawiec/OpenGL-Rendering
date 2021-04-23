#include "../SceneDrawing/SceneDrawingManager.hpp"

#include "../SceneDrawing/Frustum.hpp"

#include <glad/glad.h>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace SceneDrawing {

namespace /* anonymous */ {


ShadowMappingPass::SceneViewData createFittingShadowmapTransform(const Scene::Base::DirectionalLight& light, const glm::mat4 lightTransform, const Scene::Base::Camera& camera, const glm::mat4 cameraTransform) {

    const glm::mat4 lightWorldTransform = lightTransform;
    const glm::vec3 lightDirection = glm::normalize(lightWorldTransform * light.GetLightDirection());

    // We'll build new transform matrix for the light.
    const glm::vec3 cameraPosition = cameraTransform * glm::vec4(0, 0, 0, 1);
    // TODO: this is not perfect. If light is parallel to UP, we get messed up matrix. I need to figure something else.
    const glm::mat4 lightNewTransform = glm::lookAt(cameraPosition, cameraPosition + lightDirection, glm::vec3(0, 1, 0));

    Frustum frustum(camera); // this is frustum created in camera-space.
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
    //const glm::mat4 lightProjectionTransform = glm::ortho(-10.f, 10.f, -10.f, 10.f, -1.f, 50.f);

    ShadowMappingPass::SceneViewData viewData;
    viewData.lightViewTransform = lightNewTransform;
    viewData.lightProjectionTransform = lightProjectionTransform;

    return viewData;
}

} // namespace anonymous

SceneDrawingManager::SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight)
: _scene(scene)
, _transformProcessor(_scene)
, _deferredBuffers(windowWidth, windowHeight)
, _width(windowWidth)
, _height(windowHeight) {}

void SceneDrawingManager::Draw() {
    _transformProcessor.Update();
    ShadowMapping();
    BasePass();

    {
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _width, _height);

        _textureDrawProgram.draw(_deferredBuffers.getTexture(GraphicBuffer::Output::Albedo));
    }
}

void SceneDrawingManager::ShadowMapping()
{
    static const float TemporaryShadowMapSize = 1024;
    static const float TemporaryShadowMapFar = 100.f;
    static const float TemporaryShadowMapNear = 0.1f;

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

            ShadowMappingPass::SceneViewData viewData = createFittingShadowmapTransform(lightObject, _transformProcessor.GetNodeTransforms().at(nodeId), camera, cameraTransform);

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

                ShadowMappingPass::SceneObjectData objectData;
                objectData.objectModelTransform = _transformProcessor.GetNodeTransforms().at(sceneElement.nodeId);
                if (sceneElement.skinId != Renderer::Scene::Base::Skin::INVALID_ID) {
                    prepareSkin(sceneElement.skinId);
                    objectData.jointsArray = &_jointTransforms[sceneElement.skinId];
                }

                const auto& pipeline = _shadowMappingPassPipelineManager.GetPipeline(properties);
                const auto pipelineBinding = pipeline.Bind();
                pipeline.PrepareView(viewData);
                pipeline.PrepareElement(objectData);

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
    BasePass::SceneViewData viewData;
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

        BasePass::SceneObjectData objectData;
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
        pipeline.PrepareView(viewData);
        pipeline.PrepareElement(objectData);

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
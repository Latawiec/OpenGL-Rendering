#include "../SceneDrawing/SceneDrawingManager.hpp"

#include "../SceneDrawing/Frustum.hpp"

#include <glad/glad.h>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace SceneDrawing {

struct FittingShadowmapData {
    glm::mat4 lightViewTransform;
    
};

glm::mat4 createAlignedLightTransform(const glm::mat4& lightTransform, const glm::vec4& lightDirection, const glm::mat4& cameraTransform, const glm::mat4& cameraOrientation) {
    const glm::vec3 lightDirWorldSpace = glm::normalize(lightTransform * lightDirection);

    // We'll build new transform matrix for the light.
    const glm::vec3 cameraPosition = cameraTransform * glm::vec4(0, 0, 0, 1);
    const glm::vec3 cameraWorldRight = cameraTransform * cameraOrientation * glm::vec4(1, 0, 0, 0);
    const glm::vec3 cameraWorldUp =    cameraTransform * cameraOrientation * glm::vec4(0, 1, 0, 0);
    const glm::vec3 cameraWorldFront = cameraTransform * cameraOrientation * glm::vec4(0, 0, -1, 0);

    // I want to build perfect transform of shadowmap view - meaning side of shadowmap texture is parallel with the frustum.
    // For building the "perfect" space for shadowmap, we need to figure which 2 vectors we can use. Best is I think: two least parallel with light direction.
    const float rightDot = glm::abs(glm::dot(cameraWorldRight, lightDirWorldSpace));
    const float upDot = glm::abs(glm::dot(cameraWorldRight, lightDirWorldSpace));
    const float frontDot = glm::abs(glm::dot(cameraWorldRight, lightDirWorldSpace));

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
        glm::vec3 f = glm::normalize(lightDirWorldSpace);
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

    return lightNewTransform;
}

glm::vec3 getFurthestCoordsOfCameraFrustumInLightView(const Scene::Base::Camera& camera, const glm::mat4& cameraTransform, const glm::mat4& lightTransform) {
    Frustum frustum(camera); // this is frustum created in camera-space
    frustum.Transform(cameraTransform * camera.GetCameraOrientation()); // now it's in world-space.
    frustum.Transform(lightTransform);
    
    // Now we simply find min's and max'es from the frustum vertices in light space, and we done!
    const auto& frustumVertices = frustum.GetVertices();
    glm::vec3 maxValues = frustumVertices[0];

    for (int i=1; i<frustumVertices.size(); ++i) {
        const auto vertex = frustumVertices[i];

        maxValues.x = vertex.x > maxValues.x ? vertex.x : maxValues.x;
        maxValues.y = vertex.y > maxValues.y ? vertex.y : maxValues.y;
        maxValues.z = vertex.z > maxValues.z ? vertex.z : maxValues.z;
    }

    return maxValues;
}

glm::vec3 getNearestCoordsOfCameraFrustumInLightView(const Scene::Base::Camera& camera, const glm::mat4& cameraTransform, const glm::mat4& lightTransform) {
    Frustum frustum(camera); // this is frustum created in camera-space
    frustum.Transform(cameraTransform * camera.GetCameraOrientation()); // now it's in world-space.
    frustum.Transform(lightTransform);
    
    // Now we simply find min's and max'es from the frustum vertices in light space, and we done!
    const auto& frustumVertices = frustum.GetVertices();
    glm::vec3 minValues = frustumVertices[0];

    for (int i=1; i<frustumVertices.size(); ++i) {
        const auto vertex = frustumVertices[i];
        minValues.x = vertex.x < minValues.x ? vertex.x : minValues.x;
        minValues.y = vertex.y < minValues.y ? vertex.y : minValues.y;
        minValues.z = vertex.z < minValues.z ? vertex.z : minValues.z;
    }

    return minValues;
}

ShadowMappingPass::SharedData SceneDrawingManager::createDirectionalLightFittingShadowmapTransform(const Scene::Base::DirectionalLight& light, const glm::mat4 lightTransform, const Scene::Base::Camera& camera, const glm::mat4 cameraTransform) {

    const glm::mat4 lightNewTransform = createAlignedLightTransform(lightTransform, light.GetLightDirection(), cameraTransform, camera.GetCameraOrientation());
    const glm::vec3 minCameraFrustumCoordsLightSpace = getNearestCoordsOfCameraFrustumInLightView(camera, cameraTransform, lightNewTransform);
    const glm::vec3 maxCameraFrustumCoordsLightSpace = getFurthestCoordsOfCameraFrustumInLightView(camera, cameraTransform, lightNewTransform);

    const glm::mat4 lightProjectionTransform = glm::ortho(minCameraFrustumCoordsLightSpace.x, maxCameraFrustumCoordsLightSpace.x, minCameraFrustumCoordsLightSpace.y, maxCameraFrustumCoordsLightSpace.y, -maxCameraFrustumCoordsLightSpace.z, -minCameraFrustumCoordsLightSpace.z);
    
    ShadowMappingPass::SharedData viewData;
    viewData.lightViewTransform = lightNewTransform;
    viewData.lightProjectionTransform = lightProjectionTransform;

    return viewData;
}


ShadowMappingPass::SharedData SceneDrawingManager::createSpotLightFittingShadowmapTransform(const Scene::Base::SpotLight& light, const glm::mat4 lightTransform, const Scene::Base::Camera& camera, const glm::mat4 cameraTransform) {
    const glm::vec4 lightPos = lightTransform * glm::vec4(0, 0, 0, 1);
    const glm::mat4 lightNewTransform = glm::lookAt(glm::vec3(lightPos), glm::vec3(lightPos + lightTransform * light.GetLightDirection()), glm::vec3(0, 1, 0)); //(lightTransform, light.GetLightDirection(), cameraTransform, camera.GetCameraOrientation());
    const glm::vec3 minCameraFrustumCoordsLightSpace = getNearestCoordsOfCameraFrustumInLightView(camera, cameraTransform, lightNewTransform);
    const glm::vec3 maxCameraFrustumCoordsLightSpace = getFurthestCoordsOfCameraFrustumInLightView(camera, cameraTransform, lightNewTransform);

    const glm::mat4 lightProjectionTransform = glm::perspective(2.f * light.GetOuterConeAngle(), 1.f, -maxCameraFrustumCoordsLightSpace.z, -minCameraFrustumCoordsLightSpace.z);
    
    ShadowMappingPass::SharedData viewData;
    viewData.lightViewTransform = lightNewTransform;
    viewData.lightProjectionTransform = lightProjectionTransform;

    return viewData;
}

const Scene::SceneView& SceneDrawingManager::getActiveSceneView() const {
    return _scene.GetSceneViews()[_activeSceneViewIndex];
}

SceneDrawingManager::SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight)
: _scene(scene)
, _transformProcessor(_scene)
, _basePassBuffer(windowWidth, windowHeight)
, _contourPassBuffer(windowWidth, windowHeight)
, _lightingPassBuffer(windowWidth, windowHeight)
, _shadowMappingPassBuffer(_shadowMapWidth, _shadowMapHeight)
, _intermediateBloomBuffer(windowWidth, windowHeight)
, _finalBloomBuffer(windowWidth, windowHeight)
, _width(windowWidth)
, _height(windowHeight) {
    auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
    _framebufferPlane = Scene::Base::VertexData<Scene::Base::Layout::Sequential, Scene::Base::Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));
}

void SceneDrawingManager::Draw() {
    _transformProcessor.Update();
    ShadowMappingPass();
    BasePass();
    ContourPass();
    LightingPass();
    // TODO: post processing should be last.
    Bloom();
    CombinePass();


    // {
        // glDisable(GL_DEPTH_TEST);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glViewport(0, 0, _width, _height);

        //  _textureDrawProgram.draw(_lightingPassBuffer.getTexture(LightingPassBuffer::Output::Diffuse));

    //     // Test Shadowmap
    //     glViewport(0, 0, _basePassBuffer.GetHeight()/3.f, _basePassBuffer.GetHeight()/3.f);
    //     const auto& shadowMap = *_directionalLightShadowMaps.begin();
    //     _textureDrawProgram.draw(shadowMap.second.getTexture());

    // }
}

void SceneDrawingManager::SetWindowSize(const int windowWidth, const int windowHeight)
{
    _width = windowWidth;
    _height = windowHeight;
}

void SceneDrawingManager::SetResolution(const int pixelWidth, const int pixelHeight)
{
    _basePassBuffer = BasePass::BasePassBuffer(pixelWidth, pixelHeight);
    _contourPassBuffer = ContourPass::ContourPassBuffer(pixelWidth, pixelHeight);
    _lightingPassBuffer = LightingPass::LightingPassBuffer(pixelWidth, pixelHeight);
    _intermediateBloomBuffer = PostProcess::PostProcessBuffer(pixelWidth, pixelHeight);
    _finalBloomBuffer = PostProcess::PostProcessBuffer(pixelWidth, pixelHeight);
}

const glm::mat4& SceneDrawingManager::GetNodeWorldTransform(const Scene::Base::Node::IdType& id) const
{
    return _transformProcessor.GetNodeTransforms().at(id);
}

void SceneDrawingManager::CombinePass()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    CombinePass::CombinePipelineManager::PropertiesSet properties = 0;
    const auto& pipeline = _combinePassPipelineManager.GetPipeline(properties);

    CombinePass::SharedData data;
    data.albedoTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Albedo);
    // TODO: fix this mess.
    // Temporarily use the one from Bloom. I need to rething combine stage. It doesn't work with post-processing.
    data.diffuseTexture = _lightingPassBuffer.getTexture(LightingPass::LightingPassBuffer::Output::Diffuse);
    //data.diffuseTexture = _finalBloomBuffer.getTexture(PostProcess::PostProcessBuffer::Output::PostProcessOutput);
    data.specularTexture = _lightingPassBuffer.getTexture(LightingPass::LightingPassBuffer::Output::Specular);
    data.ditherTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Dither);
    data.contourTexture = _contourPassBuffer.getTexture(ContourPass::ContourPassBuffer::Output::ContourMap);
    data.metallicRoughnessTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::MetallicRoughness);

    const auto binding = pipeline.Bind();
    pipeline.prepareShared(data);
    pipeline.prepareIndividual();

    // Draw to screen
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
    glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
}

void SceneDrawingManager::ContourPass()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    ContourPass::ContourPipelineManager::PropertiesSet properties = 0;
    const auto& pipeline = _contourPassPipelineManager.GetPipeline(properties);

    const auto& activeView = getActiveSceneView();
    const auto& activeCamera = _scene.GetCamera(activeView.cameraId);

    ContourPass::SharedData data;
    data.silhouetteTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::SilhouetteMap);
    data.normalMapTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Normals);
    data.depthTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Depth);
    data.cameraNearFar = glm::vec2(activeCamera.GetNear(), activeCamera.GetFar());

    const auto binding = pipeline.Bind();
    pipeline.prepareShared(data);
    pipeline.prepareIndividual();

    const auto gbufferBinding = _contourPassBuffer.Bind();

    Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
    glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
}

void SceneDrawingManager::LightingPass() 
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    const auto gbufferBinding = _lightingPassBuffer.Bind();

    LightingPass::SharedData data;
    data.albedoTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Albedo);
    data.positionTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Position);
    data.normalMapTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::Normals);
    data.metallicRoughnessTexture = _basePassBuffer.getTexture(BasePass::BasePassBuffer::Output::MetallicRoughness);

    const auto& cameraNode = getActiveSceneView().nodeId;
    data.cameraPosition = _transformProcessor.GetNodeTransforms().at(cameraNode) * glm::vec4(0, 0, 0, 1);

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
            const glm::vec4 lightColor = glm::vec4(lightObject.GetColor(), lightObject.GetIntensity());
            data.directionalLightsTransforms.push_back(_shadowMappingPassBuffer.GetDirectionalLightTransform(lightId));
            data.directionalLightsDirections.push_back(lightDirection);
            data.directionalLightsColors.push_back(lightColor);
            data.directionalLightsShadowmapTextureIds.push_back(_shadowMappingPassBuffer.GetDirectionalLightShadowRenderTarget(lightId).getTexture());
        }

        if (sceneLight.spotLightId != Scene::Base::SpotLight::INVALID_ID) {
            const auto& lightId = sceneLight.spotLightId;
            const auto& lightObject = _scene.GetSpotLight(lightId);

            properties |= LightingPass::LightingPipelineManager::PipelineProperties::LIGHTTYPE_SPOT;

            const glm::mat4 lightTransform = _transformProcessor.GetNodeTransforms().at(nodeId);
            const glm::vec4 lightDirection = lightTransform * lightObject.GetLightDirection();
            const glm::vec4 lightPosition = lightTransform * glm::vec4(0, 0, 0, 1);
            const glm::vec4 lightColor = glm::vec4(lightObject.GetColor(), lightObject.GetIntensity());
            const float lightInnerConeAngle = lightObject.GetInnerConeAngle();
            const float lightOuterConeAngle = lightObject.GetOuterConeAngle();

            data.spotLightsTransforms.push_back(_shadowMappingPassBuffer.GetSpotLightTransform(lightId));
            data.spotLightsDirections.push_back(lightDirection);
            data.spotLightsPositions.push_back(lightPosition);
            data.spotLightsColors.push_back(lightColor);
            data.spotLightsInnerOuterConeAngle.push_back({lightInnerConeAngle, lightOuterConeAngle});
        }
    }

    const auto& pipeline = _lightingPassPipelineManager.GetPipeline(properties);
    const auto binding = pipeline.Bind();
    pipeline.prepareShared(data);
    pipeline.prepareIndividual();

    Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
    glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
}

void SceneDrawingManager::ShadowMappingPass()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);

    for (const auto& sceneLight : _scene.GetSceneLights()) {
        const auto& nodeId = sceneLight.nodeId;
        const auto& lightNode = _scene.GetNode(sceneLight.nodeId);


        if (sceneLight.directionalLightId != Scene::Base::DirectionalLight::INVALID_ID) {
            const auto& lightId = sceneLight.directionalLightId;
            const auto& lightObject = _scene.GetDirectionalLight(lightId);

            const auto& shadowMapBuffer = _shadowMappingPassBuffer.GetDirectionalLightShadowRenderTarget(lightId);
            const auto framebufferBinding = shadowMapBuffer.Bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            const auto& [cameraNodeId, cameraId] = getActiveSceneView();
            const auto& camera = _scene.GetCamera(cameraId);
            const auto& cameraTransform = _transformProcessor.GetNodeTransforms().at(cameraNodeId);

            ShadowMappingPass::SharedData viewData = createDirectionalLightFittingShadowmapTransform(lightObject, _transformProcessor.GetNodeTransforms().at(nodeId), camera, cameraTransform);
            // Remember it for drawing shadows
            _shadowMappingPassBuffer.SetDirectionalLightTransform(lightId, viewData.lightProjectionTransform * viewData.lightViewTransform);

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


        if (sceneLight.spotLightId != Scene::Base::SpotLight::INVALID_ID) {
            const auto& lightId = sceneLight.spotLightId;
            const auto& lightObject = _scene.GetSpotLight(lightId);

            const auto& shadowMapBuffer = _shadowMappingPassBuffer.GetSpotLightShadowRenderTarget(lightId);
            const auto framebufferBinding = shadowMapBuffer.Bind();
            glClear(GL_DEPTH_BUFFER_BIT);

            const auto& [cameraNodeId, cameraId] = getActiveSceneView();
            const auto& camera = _scene.GetCamera(cameraId);
            const auto& cameraTransform = _transformProcessor.GetNodeTransforms().at(cameraNodeId);

            ShadowMappingPass::SharedData viewData = createSpotLightFittingShadowmapTransform(lightObject, _transformProcessor.GetNodeTransforms().at(nodeId), camera, cameraTransform);
            // Remember it for drawing shadows
            _shadowMappingPassBuffer.SetSpotLightTransform(lightId, viewData.lightProjectionTransform * viewData.lightViewTransform);

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void SceneDrawingManager::BasePass()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    const auto& [nodeId, cameraId] = getActiveSceneView();
    const auto& camera = _scene.GetCamera(cameraId);
    const auto viewTransform = glm::inverse(_transformProcessor.GetNodeTransforms().at(nodeId) * camera.GetCameraOrientation());
    const auto projTransform = camera.GetProjectionTransform();

    // If I was smart enough I'd also set it up in OpenGL here once. Maybe someday...
    BasePass::SharedData viewData;
    viewData.cameraProjectionTransform = projTransform;
    viewData.cameraViewTransform = viewTransform;

    const auto binding = _basePassBuffer.Bind();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

            if (material.getTexture<Renderer::Scene::Base::Material::ETexture::MetallicRoughness>() != Renderer::Scene::Base::Texture::INVALID_ID) {
                properties |= BasePass::BasePassPipelineManager::PipelineProperties::METALLIC_ROUGHNESS_TEXTURE;
            }

            if (material.getDithering() != Renderer::Scene::Base::Material::EDithering::NONE) {
                properties |= BasePass::BasePassPipelineManager::PipelineProperties::DITHERING;
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
        if (material.getTexture<Renderer::Scene::Base::Material::ETexture::MetallicRoughness>() != Renderer::Scene::Base::Texture::INVALID_ID) {
            objectData.metallicRoughnessTexture = &_scene.GetTexture(material.getTexture<Renderer::Scene::Base::Material::ETexture::MetallicRoughness>());
        }
        if (material.getDithering() != Renderer::Scene::Base::Material::EDithering::NONE) {
            GlobalTexturesBuffer::TextureName textureName;
            switch (material.getDithering()) {
                case Renderer::Scene::Base::Material::EDithering::Bayer4x4:
                    textureName = GlobalTexturesBuffer::Bayer4x4;
                    break;
                case Renderer::Scene::Base::Material::EDithering::Bayer8x8:
                    textureName = GlobalTexturesBuffer::Bayer8x8;
                    break;
                case Renderer::Scene::Base::Material::EDithering::BlueNoise:
                    textureName = GlobalTexturesBuffer::BlueNoise;
                    break;
                case Renderer::Scene::Base::Material::EDithering::WhiteNoise:
                    textureName = GlobalTexturesBuffer::WhiteNoise;
                    break;
            }
            objectData.ditherTexture = &_globalTexturesBuffer.getTexture(textureName);
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

void SceneDrawingManager::Bloom() {

    // Draw to screen
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // Find super-bright places
    {
        PostProcess::BrightnessFilter::BrightnessFilterPipelineManager::PropertiesSet properties = PostProcess::BrightnessFilter::BrightnessFilterPipelineManager::PipelineProperties::NULLIFY_BELOW;

        PostProcess::BrightnessFilter::SharedData data;
        data.filterTreshold = 1.0f;
        data.sourceTexture = _lightingPassBuffer.getTexture(LightingPass::LightingPassBuffer::Output::Diffuse);

        const auto& pipeline = _brightnessFilterPipelineManager.GetPipeline(properties);
        const auto pipelineBinding = pipeline.Bind();

        pipeline.prepareShared(data);
        pipeline.prepareIndividual();
        
        const auto bufferBinding = _intermediateBloomBuffer.Bind();

        Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
        glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
    }

    // Horizontal Blur
    {
        PostProcess::Blur::BlurPipelineManager::PropertiesSet properties = PostProcess::Blur::BlurPipelineManager::PipelineProperties::HORIZONTAL_BLUR;

        PostProcess::Blur::SharedData data;
        data.sourceTexture = _intermediateBloomBuffer.getTexture(PostProcess::PostProcessBuffer::Output::PostProcessOutput);

        const auto& pipeline = _blurPipelineManager.GetPipeline(properties);
        const auto pipelineBinding = pipeline.Bind();

        pipeline.prepareShared(data);
        pipeline.prepareIndividual();
        const auto bufferBinding = _finalBloomBuffer.Bind();

        Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
        glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
    }

    // Vertical Blur
    {
        PostProcess::Blur::BlurPipelineManager::PropertiesSet properties = PostProcess::Blur::BlurPipelineManager::PipelineProperties::VERTICAL_BLUR;

        PostProcess::Blur::SharedData data;
        data.sourceTexture = _finalBloomBuffer.getTexture(PostProcess::PostProcessBuffer::Output::PostProcessOutput);

        const auto& pipeline = _blurPipelineManager.GetPipeline(properties);
        const auto pipelineBinding = pipeline.Bind();

        pipeline.prepareShared(data);
        pipeline.prepareIndividual();
        const auto bufferBinding = _intermediateBloomBuffer.Bind();

        Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
        glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
    }
    
    // Blend original and the result
    {
        PostProcess::Blend::BlendPipelineManager::PropertiesSet properties = PostProcess::Blend::BlendPipelineManager::PipelineProperties::ADDITIVE;

        PostProcess::Blend::SharedData data;
        data.sourcesCount = 2;
        data.sourceTextures[0] = _lightingPassBuffer.getTexture(LightingPass::LightingPassBuffer::Output::Diffuse);
        data.sourceTextures[1] = _intermediateBloomBuffer.getTexture(PostProcess::PostProcessBuffer::Output::PostProcessOutput);

        const auto& pipeline = _blendPipelineManager.GetPipeline(properties);
        const auto pipelineBinding = pipeline.Bind();

        pipeline.prepareShared(data);
        pipeline.prepareIndividual();
        const auto bufferBinding = _finalBloomBuffer.Bind();

        Renderer::Scene::Base::VertexDataBase::ScopedBinding dataBinding { _framebufferPlane };
        glDrawElements(GL_TRIANGLES, _framebufferPlane.vertexCount(), GL_UNSIGNED_INT, 0);
    }
}

} // namespace SceneDrawing
} // namespace Renderer
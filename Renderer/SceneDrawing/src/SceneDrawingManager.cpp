#include "SceneDrawing/SceneDrawingManager.hpp"

#include <glad/glad.h>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
namespace SceneDrawing {

SceneDrawingManager::SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight)
: _scene(scene)
, _transformProcessor(_scene)
, _deferredBuffers(windowWidth/2, windowHeight/2)
, _width(windowWidth)
, _height(windowHeight) {}

void SceneDrawingManager::Draw() {
    _transformProcessor.Update();
    BasePass();

    {
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _width, _height);
        // _edgeProgram.SetImageSize(_width/2, _height/2);
        // _edgeProgram.Draw(_deferredBuffers.getTexture(GraphicBuffer::Output::Normals));
        _textureDrawProgram.draw(_deferredBuffers.getTexture(GraphicBuffer::Output::Albedo));
    }
}

void SceneDrawingManager::BasePass()
{
    // Get any camera for now...
    const auto& [nodeId, cameraId] = *_scene.GetSceneViews().begin();
    const auto& camera = _scene.GetCamera(cameraId);
    const auto viewTransform = glm::inverse(_transformProcessor.GetNodeTransforms().at(nodeId) * camera.GetCameraOrientation());
    const auto projTransform = camera.GetProjectionTransform();

    Renderer::SceneDrawing::FramebufferBase::ScopedBinding bind(_deferredBuffers);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& sceneElement : _scene.GetSceneObjects()) {

        if (sceneElement.meshId == Renderer::Scene::Base::Mesh::INVALID_ID) {
            // Can't draw without geometry.
            return;
        }

        if (sceneElement.materialId == Renderer::Scene::Base::Material::INVALID_ID) {
            // Can't draw without material for now. Maybe I'll replace it with a dummy material later on.
            return;
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

        const auto& pipeline = _basePassPipelineManager.GetPipeline(properties);
        BasePass::SceneViewData viewData;
        viewData.cameraProjectionTransform = projTransform;
        viewData.cameraViewTransform = viewTransform;

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
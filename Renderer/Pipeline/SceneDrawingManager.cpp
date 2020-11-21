#include "Pipeline/SceneDrawingManager.hpp"

#include <glad/glad.h>
#include <unordered_map>

namespace Render {

SceneDrawingManager::SceneDrawingManager(const int windowWidth, const int windowHeight)
: _deferredBuffers(windowWidth/2, windowHeight/2)
, _width(windowWidth)
, _height(windowHeight) {}

void SceneDrawingManager::Draw(const Common::Scene& scene) {
    prepareSkins(scene);
    matchForQueue(scene, scene.GetNodeHierarchy());
    drawQueues();
}

void SceneDrawingManager::matchForQueue(const Common::Scene& scene, const Common::NodeLink& link) {
    const auto linkProperties = link.GetProperties();
    
    if (link.HasProperties(Common::NodeLink::CONTOUR_MESH)) {
        if (link.HasProperties(Common::NodeLink::SKINNED)) {
            queueAnimatedContourMesh(link.GetCachedTransform(), scene.GetMesh(link.GetMesh()), scene.GetSkin(link.GetSkin()));
        } else {
            queueContourMesh(link.GetCachedTransform(), scene.GetMesh(link.GetMesh()));
        }
    } 
    if (link.HasProperties(Common::NodeLink::CASTS_SHADOW)) queueShadowMesh(link.GetCachedTransform(), scene.GetMesh(link.GetMesh()));
    if (link.HasProperties(Common::NodeLink::CAMERA)) queueCamera(link.GetCachedTransform(), scene.GetCamera(link.GetCamera()));

    for (const auto& childLink : link.GetChildren()) {
        matchForQueue(scene, childLink);
    }
}

void setTransforms(
    const glm::mat4& parentTransform,
    std::vector<glm::mat4>& jointTransforms,
    const Common::Scene& scene,
    const Common::Skin& skin,
    const Common::Skin::BoneLink& boneLink)
{
    const auto boneIndex = boneLink.boneIndex;
    const Common::Skin::Bone& bone = skin.GetBones().at(boneIndex);
    const Common::Node& node = scene.GetNode(bone.nodeId);

    const glm::mat4 transform = parentTransform * node.GetTransform() * bone.inverseBindMatrix;

    jointTransforms[boneIndex] = transform;

    for (const auto& childBoneLink : boneLink.childBoneLinks) {
        setTransforms(parentTransform * node.GetTransform(), jointTransforms, scene, skin, childBoneLink);
    }
}

void SceneDrawingManager::prepareSkins(const Common::Scene& scene) {
    _preparedJointTransforms = {};
    for (const auto& [skinId, skin] : scene.GetSkins()) {
        const auto& skinHierarchy = skin.GetBonesHierarchy();
        const auto bonesCount = skin.GetBones().size();
        _preparedJointTransforms[skinId] = { 32, glm::mat4{1} };
        auto& skinJointTransforms = _preparedJointTransforms[skinId];
        setTransforms(glm::mat4{1}, skinJointTransforms, scene, skin, skin.GetBonesHierarchy());
    }
}

void SceneDrawingManager::queueContourMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
    _contourProgramExecutor.Add(transform, mesh);
}

void SceneDrawingManager::queueAnimatedContourMesh(const glm::mat4& transform, const Common::Mesh& mesh, const Common::Skin& skin) {

    _animContourProgramExecutor.Add(transform, )
}

void SceneDrawingManager::queueAnimatedContourMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
    _contourProgramExecutor.QueueMesh(transform, mesh);
}

void SceneDrawingManager::queueShadowMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
    // we'll get there!
}

void SceneDrawingManager::queueCamera(const glm::mat4& worldTransform, const Common::Camera& camera) {
    _activeCamera = {
        glm::inverse(worldTransform * camera.GetCameraOrientation()),
        camera.GetProjectionTransform()
    };
}

void SceneDrawingManager::drawQueues() {
    {
        Common::FramebufferBase::ScopedBinding bind(_deferredBuffers);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _contourProgramExecutor.Draw(_activeCamera.viewTransform, _activeCamera.projectionTransform);
        _animContourProgramExecutor.Draw(_activeCamera.viewTransform, _activeCamera.projectionTransform);
    }

    // Testing
    {
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _width, _height);
        // _edgeProgram.SetImageSize(_width/2, _height/2);
        // _edgeProgram.Draw(_deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
        _textureDrawProgram.draw(_deferredBuffers.getTexture(GraphicBuffer::Output::Position));
    }

    _contourProgramExecutor.Clear();
}

} // namespace Render
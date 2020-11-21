#include "Pipeline/SkinnedMeshQueue.hpp"

namespace Render {
namespace Pipeline {
namespace /* anonymous */ {

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

} // namespace anonymous

SkinnedMeshQueue::SkinnedMeshQueue(const Common::Scene& scene) 
: _scene(scene)
{}

void SkinnedMeshQueue::Queue(const Common::NodeLink& nodeLink)
{
    _queue.emplace(nodeLink.GetSkin(), { nodeLink.GetNode(), nodeLink.GetMesh() });
}

void SkinnedMeshQueue::Draw(const std::unordered_map<Common::Scene::NodeIdType, glm::mat4>&);
{
    //Common::Scene::SkinIdType lastUsedSkin = 0;
    for (const auto&[skinId, meshId] : _queue) {
        if (!_jointTransforms.contains(skinId)) prepareSkin(skinId);
        const auto& jointTransforms = _jointTransforms[skinId];

    }
}

void SkinnedMeshQueue::prepareSkin(const Common::Scene::SkinIdType& skinId) {
    _jointTransforms.clear();
    for (const auto& [skinId, skin] : _queue) {
        const auto& skinHierarchy = skin.GetBonesHierarchy();
        const auto bonesCount = skin.GetBones().size();
        _preparedJointTransforms[skinId] = {};
        auto& skinJointTransforms = _preparedJointTransforms[skinId];
        skinJointTransforms.fill(glm::mat4(1));
        setTransforms(glm::mat4{1}, skinJointTransforms, scene, skin, skinHierarchy);
    }
}

} // Pipeline
} // Render
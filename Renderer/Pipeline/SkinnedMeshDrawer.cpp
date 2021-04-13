#include "Pipeline/SkinnedMeshDrawer.hpp"

namespace Render {
namespace Pipeline {
namespace /* anonymous */ {

void setTransforms(
    const glm::mat4& parentTransform,
    std::array<glm::mat4, 32>& jointTransforms,
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

SkinnedMeshDrawer::SkinnedMeshDrawer(const Common::Scene& scene) 
: _scene(scene)
{}

void SkinnedMeshDrawer::Draw(const glm::mat4& view,
    const glm::mat4& proj,
    const std::unordered_map<Common::Scene::Node::IdType, glm::mat4>& nodeGlobalTransforms)
{
    const auto binding = _program.Bind();
    _program.SetView(view);
    _program.SetProjection(proj);

    for (const auto&[nodeId, skinnedMesh] : _scene.GetSkinnedMeshNodes()) {
        const auto& [meshId, skinId] = skinnedMesh;
        
        const auto& model = nodeGlobalTransforms.at(nodeId);
        _program.SetModel(model);


        if (!_jointTransforms.contains(skinId)) prepareSkin(skinId);
        const auto& jointTransforms = _jointTransforms[skinId];
        _program.SetJoints(jointTransforms);


        const auto& mesh = _scene.GetMesh(meshId);
        _program.Draw(mesh);
    }
}

void SkinnedMeshDrawer::prepareSkin(const Common::Scene::Skin::IdType& skinId) {
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

} // Pipeline
} // Render
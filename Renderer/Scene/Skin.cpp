#include "Scene/Skin.hpp"


namespace Render {
namespace Common {

Skin::Skin(std::unordered_map<JointTransformIndex, Bone>&& bones, BoneLink&& rootBoneLink)
: _bones(std::move(bones))
, _bonesHierarchy(std::move(rootBoneLink)) {}

const Skin::BoneLink& Skin::GetBonesHierarchy() const {
    return _bonesHierarchy;
}

const std::unordered_map<Skin::JointTransformIndex, Skin::Bone>&  Skin::GetBones() const {
    return _bones;
}

} // namespace Common
} // namespace Render
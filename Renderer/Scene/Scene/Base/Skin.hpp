#pragma once

#include "Node.hpp"

#include <vector>
#include <optional>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Renderer {
namespace Scene {
namespace Base {

struct Skin {
    using JointTransformIndex = std::size_t;
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    struct Bone {
        glm::mat4 inverseBindMatrix = glm::mat4{1};
        Base::Node::IdType nodeId = Base::Node::INVALID_ID;
    };

    struct BoneLink {
        JointTransformIndex boneIndex = -1;
        std::vector<BoneLink> childBoneLinks;
    };

    Skin(std::unordered_map<JointTransformIndex, Bone>&& bones, BoneLink&& rootBoneLink);

    const BoneLink& GetBonesHierarchy() const;
    const std::unordered_map<JointTransformIndex, Bone>& GetBones() const;

private:
    std::unordered_map<JointTransformIndex, Bone> _bones;
    BoneLink _bonesHierarchy;
};

} // namespace Base
} // namespace Scene
} // namespace Renderer
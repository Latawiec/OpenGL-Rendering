#pragma once

#include "IdGenerator.hpp"

#include <vector>
#include <optional>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Render {
namespace Common {

struct Node;

struct Skin {
    using JointTransformIndex = std::size_t;

    struct Bone {
        glm::mat4 inverseBindTransform = glm::mat4{1};
        IdGenerator::Type nodeId = IdGenerator::INVALID;
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

} // namespace Common
} // namespace Render
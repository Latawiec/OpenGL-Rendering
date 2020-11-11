#pragma once

#include "IdGenerator.hpp"

#include <map>

#include <glm/glm.hpp>

namespace Render {
namespace Common {

struct Node;

struct Skin {
    Skin();
    
    void AddInverseBindMatrix(IdGenerator::Type nodeId, const glm::mat4& inverseBindMatrix);
    const std::map<IdGenerator::Type, glm::mat4>& GetInverseBindMatrices() const;

private:
    std::map<IdGenerator::Type, glm::mat4> _inverseBindMatrices;
};

} // namespace Common
} // namespace Render
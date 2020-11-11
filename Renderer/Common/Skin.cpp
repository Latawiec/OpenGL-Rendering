#include "Common/Skin.hpp"


namespace Render {
namespace Common {

Skin::Skin() {}

void Skin::AddInverseBindMatrix(IdGenerator::Type nodeId, const glm::mat4& inverseBindMatrix) {
    _inverseBindMatrices[nodeId] = inverseBindMatrix;
}

const std::map<IdGenerator::Type, glm::mat4>& Skin::GetInverseBindMatrices() const {
    return _inverseBindMatrices;
}

} // namespace Common
} // namespace Render
#include "Common/IdGenerator.hpp"

namespace Render {
namespace Common {

IdGenerator::Type IdGenerator::GenerateId() {
    return _lastGeneratedId++;
}

} // namespace Common
} // namespace Render
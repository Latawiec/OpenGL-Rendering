#pragma once

#include <memory>
#include <string>

#include "Node.hpp"

namespace Render {

class Importer {

public:
    static std::unique_ptr<Node> importGltf(const std::string& filename);
};

} // namespace Render
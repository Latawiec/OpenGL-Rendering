#pragma once

#include <memory>
#include <string>

#include "Common/Scene.hpp"

namespace Render {

class Importer {
public:
    static bool importGltf(const std::string& filename, Common::Scene& scene);
};

} // namespace Render
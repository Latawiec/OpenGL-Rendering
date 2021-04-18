#pragma once

#include <unordered_map>

#include "Node.hpp"
#include "IdGenerator.hpp"
#include "Scene/DepthBuffer.hpp"

namespace Render {
namespace Lighting {

// TODOs
namespace Directional { struct Light; }
namespace Point { struct Light; }
// TODOs

struct LightingManager {
    

    LightingManager(Common::Scene& scene);



    

private:

};

} // Lighting
} // Render
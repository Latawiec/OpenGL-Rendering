#pragma once

#include <glm/glm.hpp>
#include <array>

namespace Render {
namespace MappingProgram {

using PropertiesSet = uint64_t;

enum class Properties : uint64_t {
    SKIN = 1 << 0,
    SHADOW_MAPPED = 1 << 1,
    BASE_COLOUR_TEXTURE = 1 << 2,
    NORMAL_MAP_TEXTURE = 1 << 3
};

static constexpr int MaxJointsCount = 32;
using JointsArray = std::array<glm::mat4, MaxJointsCount>;

} // namespace MappingProgram
} // namespace Render 

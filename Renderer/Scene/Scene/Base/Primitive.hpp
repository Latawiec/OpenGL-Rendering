#pragma once

#include "VertexData.hpp"

namespace Renderer {
namespace Scene {
namespace Base {

struct Primitive {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    Primitive() = default;  
    Primitive(VertexDataBase&& vertexData);

    Primitive(Primitive&& other) noexcept = default;
    Primitive& operator=(Primitive&& other) noexcept = default;

    Primitive(const Primitive& other) = delete;
    Primitive& operator=(const Primitive& other) = delete;

    const VertexDataBase& getVertexData() const;

private:
    VertexDataBase _vertexData;
};

} // namespace Base
} // namespace Scene
} // namespace Renderer
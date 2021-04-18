#pragma once

#include "VertexData.hpp"

namespace Render {
namespace Common {

struct Mesh {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    Mesh() = default;  
    Mesh(VertexDataBase&& vertexData);

    Mesh(Mesh&& other) noexcept = default;
    Mesh& operator=(Mesh&& other) noexcept = default;

    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;

    const VertexDataBase& getVertexData() const;

private:
    VertexDataBase _vertexData;
};

} // namespace Common
} // namespace Render
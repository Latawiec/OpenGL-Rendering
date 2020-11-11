#pragma once

#include "VertexData.hpp"

namespace Render {
namespace Common {

class Mesh {
    VertexDataBase _vertexData;
public:
    Mesh() = default;  
    Mesh(VertexDataBase&& vertexData);

    Mesh(Mesh&& other) noexcept = default;
    Mesh& operator=(Mesh&& other) noexcept = default;

    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;

    const VertexDataBase& getVertexData() const;
};

} // namespace Common
} // namespace Render
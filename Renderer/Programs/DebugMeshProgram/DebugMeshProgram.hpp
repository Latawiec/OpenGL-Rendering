#pragma once

#include <string_view>
#include <optional>

#include "Scene/Base/Texture.hpp"
#include "Scene/Base/VertexData.hpp"
#include "ShaderProgram.hpp"

namespace Renderer {
namespace Programs {

class DebugMeshProgram {

    constexpr static std::string_view u_viewTransform = "view";
    constexpr static std::string_view u_projectionTransform = "proj";
    constexpr static std::string_view u_modelTransform = "model";
    constexpr static std::string_view u_color = "color";
    void prepareUniforms() const;

public:
    DebugMeshProgram();

    void Draw() const;

    void SetMesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);
    void SetView(const glm::mat4& view);
    void SetProjection(const glm::mat4& proj);
    void SetModel(const glm::mat4& model);

    bool HasMesh() const;

    DebugMeshProgram(DebugMeshProgram&) = delete;
    DebugMeshProgram(DebugMeshProgram&&) = delete;
    DebugMeshProgram& operator=(DebugMeshProgram&) = delete;
    DebugMeshProgram& operator=(DebugMeshProgram&&) = delete;

private:
    glm::mat4 _view {1};
    glm::mat4 _proj {1};
    glm::mat4 _model {1};
    glm::vec3 _color = {1, 1, 1};
    std::vector<glm::vec3> _vertices;
    std::vector<unsigned int> _indices;
    std::optional<Scene::Base::VertexData<Scene::Base::Layout::Sequential, Scene::Base::Vec3>> _vertexData = std::nullopt;
    Base::ShaderProgram _shaderProgram;
};

} // namespace Programs
} // namespace Renderer 
#pragma once

#include "Scene/Base/Camera.hpp"

#include <array>
#include <glm/glm.hpp>

namespace Renderer {
namespace SceneDrawing {

struct Frustum {

    enum Vertex : uint8_t {
        LEFT_BOTTOM_NEAR = 0,
        RIGHT_BOTTOM_NEAR,
        LEFT_TOP_NEAR,
        RIGHT_TOP_NEAR,
        LEFT_BOTTOM_FAR,
        RIGHT_BOTTOM_FAR,
        LEFT_TOP_FAR,
        RIGHT_TOP_FAR,
        SIZE
    };

    Frustum(const Scene::Base::Camera& camera) {
        const float aspect = camera.GetAspectRatio();
        const float fov_y = camera.GetFov();
        const float far = camera.GetFar();
        const float near = camera.GetNear();

        const float halfHeightFar = far * glm::tan(fov_y / 2.f);
        const float halfWidthFar = halfHeightFar * aspect;

        const float halfHeightNear = halfHeightFar * near / far;
        const float halfWidthNear = halfWidthFar * near / far;

        _vertices[LEFT_BOTTOM_NEAR] =  glm::vec4(-halfWidthNear, -halfHeightNear, -near, 1);
        _vertices[RIGHT_BOTTOM_NEAR] = glm::vec4( halfWidthNear, -halfHeightNear, -near, 1);
        _vertices[LEFT_TOP_NEAR] =     glm::vec4(-halfWidthNear,  halfHeightNear, -near, 1);
        _vertices[RIGHT_TOP_NEAR] =    glm::vec4( halfWidthNear,  halfHeightNear, -near, 1);
        _vertices[LEFT_BOTTOM_FAR] =   glm::vec4(-halfWidthFar, -halfHeightFar, -far, 1);
        _vertices[RIGHT_BOTTOM_FAR] =  glm::vec4( halfWidthFar, -halfHeightFar, -far, 1);
        _vertices[LEFT_TOP_FAR] =      glm::vec4(-halfWidthFar,  halfHeightFar, -far, 1);
        _vertices[RIGHT_TOP_FAR] =     glm::vec4( halfWidthFar,  halfHeightFar, -far, 1);
    }
    
    Frustum(Frustum&& other) = default;
    Frustum& operator=(Frustum&& other) = default;

    Frustum(Frustum& other) = default;
    Frustum& operator=(Frustum& other) = default;

    const glm::vec4& GetVertex(Vertex vertex) const {
        return _vertices[vertex];
    }

    const std::array<glm::vec4, Vertex::SIZE>& GetVertices() const {
        return _vertices;
    }

    Frustum& Transform(const glm::mat4& transform) {
        for (int i=0; i<Vertex::SIZE; ++i) {
            _vertices[i] = transform * _vertices[i];
        }

        return *this;
    }

private:

    std::array<glm::vec4, Vertex::SIZE> _vertices;
};

} // namespace SceneDrawing
} // namespace Renderer
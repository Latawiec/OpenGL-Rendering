#pragma once

#ifndef NDEBUG
#include <string>
#endif

#include <vector>
#include <glm/glm.hpp>

namespace Render {
namespace Common {

struct Node {
    Node();
    Node(const glm::mat4& transform);

    void SetTransform(const glm::mat4& transform);
    const glm::mat4& GetTransform() const;

#ifndef NDEBUG
    void SetName(std::string name) {
        _name = std::move(name);
    }
    const std::string& GetName() const {
        return _name;
    }
#endif
    
private:
    glm::mat4 _transform;
#ifndef NDEBUG
    std::string _name;
#endif
};

} // namespace Common
} // namespace Render
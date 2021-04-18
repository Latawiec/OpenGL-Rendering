#pragma once

#ifndef NDEBUG
#include <string>
#endif

#include <vector>
#include <glm/glm.hpp>

namespace Render {
namespace Common {

struct Node {
    using IdType = uint64_t;
    constexpr static IdType INVALID_ID = 0;

    Node();
    Node(const glm::mat4& transform);

    void SetTransform(const glm::mat4& transform);
    const glm::mat4& GetTransform() const;
    
private:
    glm::mat4 _transform;


#ifndef NDEBUG
public:
    void SetName(std::string name) { _name = std::move(name); }
    const std::string& GetName() const { return _name; }
private:
    std::string _name;
#endif
};

} // namespace Common
} // namespace Render
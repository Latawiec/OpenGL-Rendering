#pragma once

#include <glm/glm.hpp>

class TransformBase {
    glm::mat4 _transform;
public:
    TransformBase();
    TransformBase(glm::mat4 transform);
    TransformBase& operator=(glm::mat4 transform);

    const glm::mat4& get() const;
    void set(glm::mat4 transform);

    operator glm::mat4() const;
    operator const glm::mat4&() const;
    operator glm::mat4&();
};


// Transformed<> is a wrapper around any object, to provide it with
// 3D transform (rotated/translated/scaled). It is owning/non-owning based on
// chosen template parameter.
template<class T>
class Transformed : public TransformBase {
    T _wrapped;

public:
    Transformed(T object) : TransformBase(), _wrapped(object) {}
    Transformed(T object, glm::mat4 transform) : TransformBase(transform), _wrapped(object) {}

    const T& get() const { return {_wrapped}; };
    T& get() { return {_wrapped}; };

    operator Transformed<T&>&()             { return {_wrapped}; }
    operator const Transformed<T&>&() const { return {_wrapped}; }

    operator const T&() const { return _wrapped; }
    operator T&()             { return _wrapped; }
};
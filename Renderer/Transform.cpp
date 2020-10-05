#include "Transform.hpp"


TransformBase::TransformBase()
: _transform(1)
{}

TransformBase::TransformBase(glm::mat4 transform)
: _transform(transform)
{}

TransformBase& TransformBase::operator=(glm::mat4 transform)
{
    _transform = transform;
    return *this;
}

const glm::mat4& TransformBase::get() const
{
    return _transform;
}

void TransformBase::set(glm::mat4 transform)
{
    _transform = transform;
}

TransformBase::operator glm::mat4() const
{
    return _transform;
}

TransformBase::operator const glm::mat4&() const
{
    return _transform;
}

TransformBase::operator glm::mat4&()
{
    return _transform;
}
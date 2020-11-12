#include "Common/NodeLink.hpp"

namespace Render {
namespace Common {

NodeLink::NodeLink()
: _node(IdGenerator::INVALID)
{}

NodeLink::NodeLink(IdGenerator::Type node, uint16_t properties)
: _cachedTransform(1)
, _node(node)
, _mesh(IdGenerator::INVALID)
, _camera(IdGenerator::INVALID)
, _skin(IdGenerator::INVALID)
, _properties(properties) {}

IdGenerator::Type NodeLink::GetNode() const {
    return _node;
}

void NodeLink::AddChild(NodeLink&& node) {
    _childLinks.push_back(std::move(node));
}

std::vector<NodeLink>& NodeLink::GetChildren() {
    return _childLinks;
}

const std::vector<NodeLink>& NodeLink::GetChildren() const {
    return _childLinks;
}

void NodeLink::SetMesh(IdGenerator::Type mesh) {
    _mesh = mesh;
}

IdGenerator::Type NodeLink::GetMesh() const {
    return _mesh;
}

void NodeLink::SetCamera(IdGenerator::Type camera) {
    _camera = camera;
}

IdGenerator::Type NodeLink::GetCamera() const {
    return _camera;
}

void NodeLink::SetSkin(IdGenerator::Type skin) {
    _skin = skin;
}

IdGenerator::Type NodeLink::GetSkin() const {
    return _skin;
}

void NodeLink::SetProperties(uint16_t properties) {
    _properties = properties;
}

bool NodeLink::HasProperties(const uint16_t required) const {
    return (_properties & required) == required;
}

uint16_t NodeLink::GetProperties() const {
    return _properties;
}

void NodeLink::CacheTransform(const glm::mat4& transform) {
    _cachedTransform = transform;
}

const glm::mat4& NodeLink::GetCachedTransform() const {
    return _cachedTransform;
}

} // namespace Common
} // namespace Render
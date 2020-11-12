#include "Common/Scene.hpp"

namespace Render {
namespace Common {

Scene::Scene() {
    const IdGenerator::Type rootNodeId = AddNode({});
    _sceneRoot = NodeLink(rootNodeId);
}

IdGenerator::Type Scene::AddNode(Node&& node) {
    const auto id = _nodeIdGenerator.GenerateId();
    _nodes.insert({ id, std::move(node) });
    return id;
}

Node& Scene::GetNode(const IdGenerator::Type& id) {
    return _nodes.at(id);
}

const Node& Scene::GetNode(const IdGenerator::Type& id) const {
    return _nodes.at(id);
}

IdGenerator::Type Scene::AddMesh(Mesh&& mesh) {
    const auto id = _meshIdGenerator.GenerateId();
    _meshes.insert({ id, std::move(mesh) });
    return id;
}

Mesh& Scene::GetMesh(const IdGenerator::Type& id) {
    return _meshes.at(id);
}

const Mesh& Scene::GetMesh(const IdGenerator::Type& id) const {
    return _meshes.at(id);
}

IdGenerator::Type Scene::AddCamera(Camera&& camera) {
    const auto id = _cameraIdGenerator.GenerateId();
    _cameras.insert({ id, std::move(camera) });
    return id;
}

Camera& Scene::GetCamera(const IdGenerator::Type& id) {
    return _cameras.at(id);
}

const Camera& Scene::GetCamera(const IdGenerator::Type& id) const {
    return _cameras.at(id);
}

IdGenerator::Type Scene::AddSkin(Skin&& skin) {
    const auto id = _skinIdGenerator.GenerateId();
    _skins.insert({ id, std::move(skin) });
    return id;
}

Skin& Scene::GetSkin(const IdGenerator::Type& id) {
    return _skins.at(id);
}

const Skin& Scene::GetSkin(const IdGenerator::Type& id) const {
    return _skins.at(id);
}

void Scene::AddNodeHierarchy(NodeLink&& nodeLink) {
    _sceneRoot.AddChild(std::move(nodeLink));
}

NodeLink& Scene::GetNodeHierarchy() {
    return _sceneRoot;
}

const NodeLink& Scene::GetNodeHierarchy() const {
    return _sceneRoot;
}

void Scene::UpdateNodes() {
    const glm::mat4 initialTransform{1};
    updateLink(initialTransform, _sceneRoot);
}

void Scene::updateLink(const glm::mat4& parentTransform, NodeLink& link) {
    const Node& linkNode = _nodes[link.GetNode()];
    const glm::mat4 transform = linkNode.GetTransform() * parentTransform;
    link.CacheTransform(transform);
    for (auto& linkChild : link.GetChildren()) {
        updateLink(transform, linkChild);
    }
}

#ifndef NDEBUG
std::ostream& operator<<(std::ostream& stream, Scene& scene) {
    const NodeLink& root = scene.GetNodeHierarchy();
    Scene::printNodeLink(stream, scene, root);
    return stream;
}
#endif

} // namespace Common
} // namespace Render


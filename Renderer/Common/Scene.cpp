#include "Common/Scene.hpp"

namespace Render {
namespace Common {

Scene::Scene() {}

IdGenerator::Type Scene::AddNode(Node&& node) {
    const auto id = _nodeIdGenerator.GenerateId();
    _nodes.insert({ id, std::move(node) });
    return id;
}

IdGenerator::Type Scene::AddMesh(Mesh&& mesh) {
    const auto id = _meshIdGenerator.GenerateId();
    _meshes.insert({ id, std::move(mesh) });
    return id;
}

IdGenerator::Type Scene::AddCamera(Camera&& camera) {
    const auto id = _cameraIdGenerator.GenerateId();
    _cameras.insert({ id, std::move(camera) });
    return id;
}

IdGenerator::Type Scene::AddSkin(Skin&& skin) {
    const auto id = _skinIdGenerator.GenerateId();
    _skins.insert({ id, std::move(skin) });
    return id;
}

void Scene::AddNodeHierarchy(NodeLink&& nodeLink) {
    _sceneRoot.AddChild(std::move(nodeLink));
}

const NodeLink& Scene::GetNodeHierarchy() const {
    return _sceneRoot;
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


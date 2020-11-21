#include "Common/Scene.hpp"

namespace Render {
namespace Common {

Scene::Scene() {
    const IdGenerator::Type rootNodeId = AddNode({});
    _sceneRoot = NodeLink(rootNodeId);
}

Scene::NodeIdType Scene::AddNode(Node&& node) {
    const auto id = _nodeIdGenerator.GenerateId();
    _nodes.insert({ id, std::move(node) });
    return id;
}

Node& Scene::GetNode(const Scene::NodeIdType& id) {
    return _nodes.at(id);
}

const Node& Scene::GetNode(const Scene::NodeIdType& id) const {
    return _nodes.at(id);
}

Scene::MeshIdType Scene::AddMesh(Mesh&& mesh) {
    const auto id = _meshIdGenerator.GenerateId();
    _meshes.insert({ id, std::move(mesh) });
    return id;
}

Mesh& Scene::GetMesh(const Scene::MeshIdType& id) {
    return _meshes.at(id);
}

const Mesh& Scene::GetMesh(const Scene::MeshIdType& id) const {
    return _meshes.at(id);
}

Scene::CameraIdType Scene::AddCamera(Camera&& camera) {
    const auto id = _cameraIdGenerator.GenerateId();
    _cameras.insert({ id, std::move(camera) });
    return id;
}

Camera& Scene::GetCamera(const Scene::CameraIdType& id) {
    return _cameras.at(id);
}

const Camera& Scene::GetCamera(const Scene::CameraIdType& id) const {
    return _cameras.at(id);
}

Scene::SkinIdType Scene::AddSkin(Skin&& skin) {
    const auto id = _skinIdGenerator.GenerateId();
    _skins.insert({ id, std::move(skin) });
    return id;
}

Skin& Scene::GetSkin(const Scene::SkinIdType& id) {
    return _skins.at(id);
}

const Skin& Scene::GetSkin(const Scene::SkinIdType& id) const {
    return _skins.at(id);
}

const std::unordered_map<Scene::SkinIdType, Skin>& Scene::GetSkins() const {
    return _skins;
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

void Scene::AttachCamera(const NodeIdType& nodeId, const CameraIdType& cameraId) {
    _cameraNodes[nodeId] = cameraId;
}

void Scene::DetachCamera(const NodeIdType& nodeId) {
    _cameraNodes.erase(nodeId);
}

const std::unordered_map<Scene::NodeIdType, Scene::CameraIdType>& Scene::GetCameraNodes() const {
    return _cameraNodes;
}

void Scene::AttachStaticMesh(const NodeIdType& nodeId, const MeshIdType& meshId) {
    _staticMeshNodes[nodeId] = meshId;
}

void Scene::DetachStaticMesh(const NodeIdType& nodeId) {
    _staticMeshNodes.erase(nodeId);
}

const std::unordered_map<Scene::NodeIdType, Scene::MeshIdType>& Scene::GetStaticMeshNodes() const {
    return _staticMeshNodes;
}

void Scene::AttachSkinnedMesh(const NodeIdType& nodeId, const Scene::SkinnedMesh skinnedMesh) {
    _skinnedMeshNodes[nodeId] = skinnedMesh;
}

void Scene::DetachSkinnedMesh(const NodeIdType& nodeId) {
    _skinnedMeshNodes.erase(nodeId);
}

const std::unordered_map<Scene::NodeIdType, Scene::SkinnedMesh>& Scene::GetSkinnedMeshNodes() const {
    return _skinnedMeshNodes;
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


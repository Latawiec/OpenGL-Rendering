#include "Common/Scene.hpp"

namespace Render {
namespace Common {

Scene::Scene() {
    const IdGenerator::Type rootNodeId = AddNode({});
    _sceneRoot = NodeLink(rootNodeId);
}

Scene::Node::IdType Scene::AddNode(Node&& node) {
    const auto id = _nodeIdGenerator.GenerateId();
    _nodes.insert({ id, std::move(node) });
    return id;
}

Node& Scene::GetNode(const Scene::Node::IdType& id) {
    return _nodes.at(id);
}

const Node& Scene::GetNode(const Scene::Node::IdType& id) const {
    return _nodes.at(id);
}

Scene::Mesh::IdType Scene::AddMesh(Mesh&& mesh) {
    const auto id = _meshIdGenerator.GenerateId();
    _meshes.insert({ id, std::move(mesh) });
    return id;
}

Mesh& Scene::GetMesh(const Scene::Mesh::IdType& id) {
    return _meshes.at(id);
}

const Mesh& Scene::GetMesh(const Scene::Mesh::IdType& id) const {
    return _meshes.at(id);
}

Scene::Texture::IdType Scene::AddTexture(Texture&& texture) {
    const auto id = _textureIdGenerator.GenerateId();
    _textures.emplace(id, std::move(texture));
    return id;
}

Texture& Scene::GetTexture(const Texture::IdType& id) {
    return _textures.at(id);
}

const Texture& Scene::GetTexture(const Texture::IdType& id) const {
    return _textures.at(id);
}

Scene::Material::IdType Scene::AddMaterial(BasicMaterial&& material) {
    const auto id = _materialIdGenerator.GenerateId();
    _materials.emplace(id, std::move(material));
    return id;
}

Scene::BasicMaterial& Scene::GetMaterial(const Material::IdType& id) {
    return _materials.at(id);
}

const Scene::BasicMaterial& Scene::GetMaterial(const Material::IdType& id) const {
    return _materials.at(id);
}

Scene::Camera::IdType Scene::AddCamera(Camera&& camera) {
    const auto id = _cameraIdGenerator.GenerateId();
    _cameras.insert({ id, std::move(camera) });
    return id;
}

Camera& Scene::GetCamera(const Scene::Camera::IdType& id) {
    return _cameras.at(id);
}

const Camera& Scene::GetCamera(const Scene::Camera::IdType& id) const {
    return _cameras.at(id);
}

Scene::Skin::IdType Scene::AddSkin(Skin&& skin) {
    const auto id = _skinIdGenerator.GenerateId();
    _skins.insert({ id, std::move(skin) });
    return id;
}

Skin& Scene::GetSkin(const Scene::Skin::IdType& id) {
    return _skins.at(id);
}

const Skin& Scene::GetSkin(const Scene::Skin::IdType& id) const {
    return _skins.at(id);
}

const std::unordered_map<Scene::Skin::IdType, Skin>& Scene::GetSkins() const {
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

void Scene::AttachCamera(const Node::IdType& nodeId, const Camera::IdType& cameraId) {
    _cameraNodes[nodeId] = cameraId;
}

void Scene::DetachCamera(const Node::IdType& nodeId) {
    _cameraNodes.erase(nodeId);
}

const std::unordered_map<Scene::Node::IdType, Scene::Camera::IdType>& Scene::GetCameraNodes() const {
    return _cameraNodes;
}

void Scene::AttachStaticMesh(const Node::IdType& nodeId, const Mesh::IdType& meshId) {
    _staticMeshNodes[nodeId] = meshId;
}

void Scene::DetachStaticMesh(const Node::IdType& nodeId) {
    _staticMeshNodes.erase(nodeId);
}

const std::unordered_map<Scene::Node::IdType, Scene::Mesh::IdType>& Scene::GetStaticMeshNodes() const {
    return _staticMeshNodes;
}

void Scene::AttachSkinnedMesh(const Node::IdType& nodeId, const Scene::SkinnedMesh skinnedMesh) {
    _skinnedMeshNodes[nodeId] = skinnedMesh;
}

void Scene::DetachSkinnedMesh(const Node::IdType& nodeId) {
    _skinnedMeshNodes.erase(nodeId);
}

const std::unordered_map<Scene::Node::IdType, Scene::SkinnedMesh>& Scene::GetSkinnedMeshNodes() const {
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


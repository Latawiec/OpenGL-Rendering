#include "Scene/Scene.hpp"

namespace Render {
namespace Common {

Scene::Scene() {
    const Node::IdType rootNodeId = AddNode({});
    _sceneRoot = NodeLink(rootNodeId);
}

Node::IdType Scene::AddNode(Node&& node) {
    const auto id = _nodeIdGenerator.GenerateId();
    _nodes.insert({ id, std::move(node) });
    return id;
}

Node& Scene::GetNode(const Node::IdType& id) {
    return _nodes.at(id);
}

const Node& Scene::GetNode(const Node::IdType& id) const {
    return _nodes.at(id);
}

Mesh::IdType Scene::AddMesh(Mesh&& mesh) {
    const auto id = _meshIdGenerator.GenerateId();
    _meshes.insert({ id, std::move(mesh) });
    return id;
}

Mesh& Scene::GetMesh(const Mesh::IdType& id) {
    return _meshes.at(id);
}

const Mesh& Scene::GetMesh(const Mesh::IdType& id) const {
    return _meshes.at(id);
}

Texture::IdType Scene::AddTexture(Texture&& texture) {
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

Material::IdType Scene::AddMaterial(Material&& material) {
    const auto id = _materialIdGenerator.GenerateId();
    _materials.emplace(id, std::move(material));
    return id;
}

Material& Scene::GetMaterial(const Material::IdType& id) {
    return _materials.at(id);
}

const Material& Scene::GetMaterial(const Material::IdType& id) const {
    return _materials.at(id);
}

Camera::IdType Scene::AddCamera(Camera&& camera) {
    const auto id = _cameraIdGenerator.GenerateId();
    _cameras.insert({ id, std::move(camera) });
    return id;
}

Camera& Scene::GetCamera(const Camera::IdType& id) {
    return _cameras.at(id);
}

const Camera& Scene::GetCamera(const Camera::IdType& id) const {
    return _cameras.at(id);
}

Skin::IdType Scene::AddSkin(Skin&& skin) {
    const auto id = _skinIdGenerator.GenerateId();
    _skins.insert({ id, std::move(skin) });
    return id;
}

Skin& Scene::GetSkin(const Skin::IdType& id) {
    return _skins.at(id);
}

const Skin& Scene::GetSkin(const Skin::IdType& id) const {
    return _skins.at(id);
}

const std::unordered_map<Skin::IdType, Skin>& Scene::GetSkins() const {
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

void Scene::AddSceneObject(const SceneObject& sceneObject) {
    _sceneObjects.push_back(sceneObject);
}

const std::vector<Scene::SceneObject>& Scene::GetSceneObjects() const {
    return _sceneObjects;
}

void Scene::AddSceneView(const Scene::SceneView& sceneView) {
    _sceneViews.push_back(sceneView);
}

const std::vector<Scene::SceneView>& Scene::GetSceneViews() const {
    return _sceneViews;
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


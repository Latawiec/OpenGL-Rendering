#include "../Scene/Scene.hpp"

namespace Renderer {
namespace Scene {

Scene::Scene() {
    const Base::Node::IdType rootNodeId = AddNode({});
    _sceneRoot = NodeLink(rootNodeId);
}

Base::Node::IdType Scene::AddNode(Base::Node&& node) {
    const auto id = _nodeIdGenerator.GenerateId();
    _nodes.insert({ id, std::move(node) });
    return id;
}

Base::Node& Scene::GetNode(const Base::Node::IdType& id) {
    return _nodes.at(id);
}

const Base::Node& Scene::GetNode(const Base::Node::IdType& id) const {
    return _nodes.at(id);
}

Base::Mesh::IdType Scene::AddMesh(Base::Mesh&& mesh) {
    const auto id = _meshIdGenerator.GenerateId();
    _meshes.insert({ id, std::move(mesh) });
    return id;
}

Base::Mesh& Scene::GetMesh(const Base::Mesh::IdType& id) {
    return _meshes.at(id);
}

const Base::Mesh& Scene::GetMesh(const Base::Mesh::IdType& id) const {
    return _meshes.at(id);
}

Base::Texture::IdType Scene::AddTexture(Base::Texture&& texture) {
    const auto id = _textureIdGenerator.GenerateId();
    _textures.emplace(id, std::move(texture));
    return id;
}

Base::Texture& Scene::GetTexture(const Base::Texture::IdType& id) {
    return _textures.at(id);
}

const Base::Texture& Scene::GetTexture(const Base::Texture::IdType& id) const {
    return _textures.at(id);
}

Base::Material::IdType Scene::AddMaterial(Base::Material&& material) {
    const auto id = _materialIdGenerator.GenerateId();
    _materials.emplace(id, std::move(material));
    return id;
}

Base::Material& Scene::GetMaterial(const Base::Material::IdType& id) {
    return _materials.at(id);
}

const Base::Material& Scene::GetMaterial(const Base::Material::IdType& id) const {
    return _materials.at(id);
}

Base::Camera::IdType Scene::AddCamera(Base::Camera&& camera) {
    const auto id = _cameraIdGenerator.GenerateId();
    _cameras.insert({ id, std::move(camera) });
    return id;
}

Base::Camera& Scene::GetCamera(const Base::Camera::IdType& id) {
    return _cameras.at(id);
}

const Base::Camera& Scene::GetCamera(const Base::Camera::IdType& id) const {
    return _cameras.at(id);
}

Base::Skin::IdType Scene::AddSkin(Base::Skin&& skin) {
    const auto id = _skinIdGenerator.GenerateId();
    _skins.insert({ id, std::move(skin) });
    return id;
}

Base::Skin& Scene::GetSkin(const Base::Skin::IdType& id) {
    return _skins.at(id);
}

const Base::Skin& Scene::GetSkin(const Base::Skin::IdType& id) const {
    return _skins.at(id);
}

const std::unordered_map<Base::Skin::IdType, Base::Skin>& Scene::GetSkins() const {
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

const std::vector<SceneObject>& Scene::GetSceneObjects() const {
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

} // namespace Scene
} // namespace Renderer


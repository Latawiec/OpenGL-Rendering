#pragma once

#ifndef NDEBUG
#include <iostream>
#endif

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Skin.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "NodeLink.hpp"
#include "IdGenerator.hpp"

#include <array>
#include <unordered_map>

namespace Render {
namespace Common {

struct Scene {

    // Helper
    struct SkinnedMesh {
        Mesh::IdType meshId;
        Skin::IdType skinId;
    };

    struct SceneObject {
        Node::IdType nodeId = Node::INVALID_ID;
        Mesh::IdType meshId = Mesh::INVALID_ID;
        Skin::IdType skinId = Skin::INVALID_ID;
        Material::IdType materialId = Material::INVALID_ID;
    };

    struct SceneView {
        Node::IdType nodeId = Node::INVALID_ID;
        Camera::IdType cameraId = Camera::INVALID_ID;
    };

    Scene();

    Node::IdType AddNode(Node&& node);
    Node& GetNode(const Node::IdType& id);
    const Node& GetNode(const Node::IdType& id) const;

    Mesh::IdType AddMesh(Mesh&& mesh);
    Mesh& GetMesh(const Mesh::IdType& id);
    const Mesh& GetMesh(const Mesh::IdType& id) const;

    Texture::IdType AddTexture(Texture&& texture);
    Texture& GetTexture(const Texture::IdType& id);
    const Texture& GetTexture(const Texture::IdType& id) const;

    Material::IdType AddMaterial(Material&& material);
    Material& GetMaterial(const Material::IdType& id);
    const Material& GetMaterial(const Material::IdType& id) const;

    Camera::IdType AddCamera(Camera&& camera);
    Camera& GetCamera(const Camera::IdType& id);
    const Camera& GetCamera(const Camera::IdType& id) const;

    Skin::IdType AddSkin(Skin&& skin);
    Skin& GetSkin(const Skin::IdType& id);
    const Skin& GetSkin(const Skin::IdType& id) const;
    const std::unordered_map<Skin::IdType, Skin>& GetSkins() const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    void AddSceneObject(const SceneObject& sceneObject);
    const std::vector<SceneObject>& GetSceneObjects() const;

    void AddSceneView(const SceneView& sceneView);
    const std::vector<SceneView>& GetSceneViews() const;

private:
    IdGenerator<Node::IdType> _nodeIdGenerator;
    IdGenerator<Mesh::IdType> _meshIdGenerator;
    IdGenerator<Texture::IdType> _textureIdGenerator;
    IdGenerator<Material::IdType> _materialIdGenerator;
    IdGenerator<Camera::IdType> _cameraIdGenerator;
    IdGenerator<Skin::IdType> _skinIdGenerator;

    // Raw data
    std::unordered_map<Node::IdType, Node> _nodes;
    std::unordered_map<Mesh::IdType, Mesh> _meshes;
    std::unordered_map<Camera::IdType, Camera> _cameras;
    std::unordered_map<Skin::IdType, Skin> _skins;
    std::unordered_map<Texture::IdType, Texture> _textures;
    std::unordered_map<Material::IdType, Material> _materials;

    // Data connections
    std::vector<SceneObject> _sceneObjects;
    std::vector<SceneView> _sceneViews;

    NodeLink _sceneRoot;

#ifndef NDEBUG
public:
    static void printNodeLink(std::ostream& stream, Scene& scene, const NodeLink& nodeLink, int level = 0) {
        stream << std::string(level, '-') << scene._nodes[nodeLink.GetNode()].GetName() << '\n';
        for (const auto& childNodeLink : nodeLink.GetChildren()) {
            printNodeLink(stream, scene, childNodeLink, level + 1);
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, Scene& scene);
#endif
};

} // namespace Common
} // namespace Render
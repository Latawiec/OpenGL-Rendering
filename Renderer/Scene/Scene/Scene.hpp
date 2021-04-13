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

    // void AddDirectionalLight(Directional::Light light);
    // void AddShadow(const Light::IdType& id, uint8_t quality);
    // void RemoveShadow(const Light::IdType& id);
    // Directional::Light& GetDirectionalLight(const Light::IdType& id);
    // const Directional::Light& GetDirectionalLight(const Light::IdType& id) const;
    // const DepthBuffer& GetDirectionalShadow(const Light::IdType& id) const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    void AttachCamera(const Node::IdType& nodeId, const Camera::IdType& cameraId);
    void DetachCamera(const Node::IdType& nodeId);
    const std::unordered_map<Node::IdType, Camera::IdType>& GetCameraNodes() const;

    void AttachStaticMesh(const Node::IdType& nodeId, const Mesh::IdType& meshId);
    void DetachStaticMesh(const Node::IdType& nodeId);
    const std::unordered_map<Node::IdType, Mesh::IdType>& GetStaticMeshNodes() const;

    void AttachSkinnedMesh(const Node::IdType& nodeId, const SkinnedMesh skinnedMesh);
    void DetachSkinnedMesh(const Node::IdType& nodeId);
    const std::unordered_map<Node::IdType, SkinnedMesh>& GetSkinnedMeshNodes() const;

    // void AttachLight(const Node::IdType& nodeId, const Light::IdType& lightId);
    // void DetachLight(const Node::IdType& nodeId, const Light::IdType& lightId);
    // const std::unordered_map<Node::IdType, Directional::Light>& GetDirectionalLightNodes() const;
    // const std::unordered_map<Light::IdType, DepthBuffer>& GetDirectionalShadowMaps() const;

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

    // std::unordered_map<Light::IdType, Directional::Light> _directionalLights;
    // std::unordered_map<Light::IdType, DepthBuffer> _directionalShadowMaps;

    // Data connections
    std::unordered_map<Node::IdType, Camera::IdType> _cameraNodes;
    std::unordered_map<Node::IdType, Mesh::IdType> _staticMeshNodes;
    std::unordered_map<Node::IdType, SkinnedMesh> _skinnedMeshNodes;
    // std::unordered_map<Node::IdType, Light::IdType> _directionalLightNodes;


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
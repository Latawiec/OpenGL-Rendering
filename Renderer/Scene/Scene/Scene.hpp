#pragma once

#ifndef NDEBUG
#include <iostream>
#endif

#include "Base/Camera.hpp"
#include "Base/Material.hpp"
#include "Base/Mesh.hpp"
#include "Base/Node.hpp"
#include "Base/Skin.hpp"
#include "Base/Texture.hpp"
#include "NodeLink.hpp"
#include "SceneObject.hpp"

#include <id_generator.hpp>

#include <array>
#include <unordered_map>

namespace Renderer {
namespace Scene {

struct Scene {

    struct SceneView {
        Base::Node::IdType nodeId = Base::Node::INVALID_ID;
        Base::Camera::IdType cameraId = Base::Camera::INVALID_ID;
    };

    Scene();

    Base::Node::IdType AddNode(Base::Node&& node);
    Base::Node& GetNode(const Base::Node::IdType& id);
    const Base::Node& GetNode(const Base::Node::IdType& id) const;

    Base::Mesh::IdType AddMesh(Base::Mesh&& mesh);
    Base::Mesh& GetMesh(const Base::Mesh::IdType& id);
    const Base::Mesh& GetMesh(const Base::Mesh::IdType& id) const;

    Base::Texture::IdType AddTexture(Base::Texture&& texture);
    Base::Texture& GetTexture(const Base::Texture::IdType& id);
    const Base::Texture& GetTexture(const Base::Texture::IdType& id) const;

    Base::Material::IdType AddMaterial(Base::Material&& material);
    Base::Material& GetMaterial(const Base::Material::IdType& id);
    const Base::Material& GetMaterial(const Base::Material::IdType& id) const;

    Base::Camera::IdType AddCamera(Base::Camera&& camera);
    Base::Camera& GetCamera(const Base::Camera::IdType& id);
    const Base::Camera& GetCamera(const Base::Camera::IdType& id) const;

    Base::Skin::IdType AddSkin(Base::Skin&& skin);
    Base::Skin& GetSkin(const Base::Skin::IdType& id);
    const Base::Skin& GetSkin(const Base::Skin::IdType& id) const;
    const std::unordered_map<Base::Skin::IdType, Base::Skin>& GetSkins() const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    void AddSceneObject(const SceneObject& sceneObject);
    const std::vector<SceneObject>& GetSceneObjects() const;

    void AddSceneView(const SceneView& sceneView);
    const std::vector<SceneView>& GetSceneViews() const;

private:
    Utils::IdGenerator<Base::Node::IdType> _nodeIdGenerator;
    Utils::IdGenerator<Base::Mesh::IdType> _meshIdGenerator;
    Utils::IdGenerator<Base::Texture::IdType> _textureIdGenerator;
    Utils::IdGenerator<Base::Material::IdType> _materialIdGenerator;
    Utils::IdGenerator<Base::Camera::IdType> _cameraIdGenerator;
    Utils::IdGenerator<Base::Skin::IdType> _skinIdGenerator;

    // Raw data
    std::unordered_map<Base::Node::IdType, Base::Node> _nodes;
    std::unordered_map<Base::Mesh::IdType, Base::Mesh> _meshes;
    std::unordered_map<Base::Camera::IdType, Base::Camera> _cameras;
    std::unordered_map<Base::Skin::IdType, Base::Skin> _skins;
    std::unordered_map<Base::Texture::IdType, Base::Texture> _textures;
    std::unordered_map<Base::Material::IdType, Base::Material> _materials;

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

} // namespace Scene
} // namespace Renderer
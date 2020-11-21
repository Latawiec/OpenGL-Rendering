#pragma once

#ifndef NDEBUG
#include <iostream>
#endif

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Skin.hpp"
#include "NodeLink.hpp"
#include "IdGenerator.hpp"

#include <unordered_map>

namespace Render {
namespace Common {

struct Scene {
    using NodeIdType = IdGenerator::Type;
    using MeshIdType = IdGenerator::Type;
    using CameraIdType = IdGenerator::Type;
    using SkinIdType = IdGenerator::Type;


    Scene();

    NodeIdType AddNode(Node&& node);
    Node& GetNode(const NodeIdType& id);
    const Node& GetNode(const NodeIdType& id) const;

    MeshIdType AddMesh(Mesh&& mesh);
    Mesh& GetMesh(const MeshIdType& id);
    const Mesh& GetMesh(const MeshIdType& id) const;

    CameraIdType AddCamera(Camera&& camera);
    Camera& GetCamera(const CameraIdType& id);
    const Camera& GetCamera(const CameraIdType& id) const;

    SkinIdType AddSkin(Skin&& skin);
    Skin& GetSkin(const SkinIdType& id);
    const Skin& GetSkin(const SkinIdType& id) const;
    const std::unordered_map<SkinIdType, Skin>& GetSkins() const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    CameraIdType GetActiveCamera() const;

    void UpdateNodes();

private:
    IdGenerator _nodeIdGenerator;
    IdGenerator _meshIdGenerator;
    IdGenerator _cameraIdGenerator;
    IdGenerator _skinIdGenerator;

    std::unordered_map<NodeIdType, Node> _nodes;
    std::unordered_map<MeshIdType, Mesh> _meshes;
    std::unordered_map<CameraIdType, Camera> _cameras;
    std::unordered_map<SkinIdType, Skin> _skins;

    NodeLink _sceneRoot;

    void updateLink(const glm::mat4& parentTransform, NodeLink& link);

    // smthing smthing activeCamera; don't know if pointer or ref yet. Can there be no active camera?...
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
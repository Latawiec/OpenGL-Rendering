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

    // Helper
    struct SkinnedMesh {
        MeshIdType meshId;
        SkinIdType skinId;
    };

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

    void AttachCamera(const NodeIdType& nodeId, const CameraIdType& cameraId);
    void DetachCamera(const NodeIdType& nodeId);
    const std::unordered_map<NodeIdType, CameraIdType>& GetCameraNodes() const;

    void AttachStaticMesh(const NodeIdType& nodeId, const MeshIdType& meshId);
    void DetachStaticMesh(const NodeIdType& nodeId);
    const std::unordered_map<NodeIdType, MeshIdType>& GetStaticMeshNodes() const;

    void AttachSkinnedMesh(const NodeIdType& nodeId, const SkinnedMesh skinnedMesh);
    void DetachSkinnedMesh(const NodeIdType& nodeId);
    const std::unordered_map<NodeIdType, SkinnedMesh>& GetSkinnedMeshNodes() const;

private:
    IdGenerator _nodeIdGenerator;
    IdGenerator _meshIdGenerator;
    IdGenerator _cameraIdGenerator;
    IdGenerator _skinIdGenerator;

    // Raw data
    std::unordered_map<NodeIdType, Node> _nodes;
    std::unordered_map<MeshIdType, Mesh> _meshes;
    std::unordered_map<CameraIdType, Camera> _cameras;
    std::unordered_map<SkinIdType, Skin> _skins;

    // Data connections
    std::unordered_map<NodeIdType, CameraIdType> _cameraNodes;
    std::unordered_map<NodeIdType, MeshIdType> _staticMeshNodes;
    std::unordered_map<NodeIdType, SkinnedMesh> _skinnedMeshNodes;

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
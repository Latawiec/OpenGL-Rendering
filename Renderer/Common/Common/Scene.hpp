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

    Scene();

    IdGenerator::Type AddNode(Node&& node);
    Node& GetNode(const IdGenerator::Type& id);
    const Node& GetNode(const IdGenerator::Type& id) const;

    IdGenerator::Type AddMesh(Mesh&& mesh);
    Mesh& GetMesh(const IdGenerator::Type& id);
    const Mesh& GetMesh(const IdGenerator::Type& id) const;

    IdGenerator::Type AddCamera(Camera&& camera);
    Camera& GetCamera(const IdGenerator::Type& id);
    const Camera& GetCamera(const IdGenerator::Type& id) const;

    IdGenerator::Type AddSkin(Skin&& skin);
    Skin& GetSkin(const IdGenerator::Type& id);
    const Skin& GetSkin(const IdGenerator::Type& id) const;
    const std::unordered_map<IdGenerator::Type, Skin>& GetSkins() const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    void UpdateNodes();

private:
    IdGenerator _nodeIdGenerator;
    IdGenerator _meshIdGenerator;
    IdGenerator _cameraIdGenerator;
    IdGenerator _skinIdGenerator;

    std::unordered_map<IdGenerator::Type, Node> _nodes;
    std::unordered_map<IdGenerator::Type, Mesh> _meshes;
    std::unordered_map<IdGenerator::Type, Camera> _cameras;
    std::unordered_map<IdGenerator::Type, Skin> _skins;

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
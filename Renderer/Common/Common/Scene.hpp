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

#include <map>

namespace Render {
namespace Common {

struct Scene {

    Scene();

    IdGenerator::Type AddNode(Node&& node);
    IdGenerator::Type AddMesh(Mesh&& mesh);
    IdGenerator::Type AddCamera(Camera&& camera);
    IdGenerator::Type AddSkin(Skin&& skin);
    void AddNodeHierarchy(NodeLink&& nodeLink);

    const NodeLink& GetNodeHierarchy() const;

private:
    std::map<IdGenerator::Type, Node> _nodes;
    std::map<IdGenerator::Type, Mesh> _meshes;
    std::map<IdGenerator::Type, Camera> _cameras;
    std::map<IdGenerator::Type, Skin> _skins;

    NodeLink _sceneRoot;

    IdGenerator _nodeIdGenerator;
    IdGenerator _meshIdGenerator;
    IdGenerator _cameraIdGenerator;
    IdGenerator _skinIdGenerator;

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
#pragma once

#include "Program.hpp"
#include "Node.hpp"
#include <overloaded.hpp>

namespace Render {

class DrawingManager {
    template<class Program, class Mesh>
    class DrawingExecutor {
        using TransformedMesh = std::pair<glm::mat4, const Mesh&>;
        Program _program;
        std::vector<TransformedMesh> _drawingQueue;
    public:
        void QueueMesh(glm::mat4 transform, const Mesh& mesh) {
            _drawingQueue.push_back({std::move(transform), mesh});
        }
        
        void Draw(const glm::mat4& view, const glm::mat4& projection) {
            _program.Draw(view, projection, _drawingQueue);
            _drawingQueue.clear();
        }
    };

    DrawingExecutor<Contour::Program, Contour::Mesh> _contourProgramExecutor;
    // more programs soon...

    void queueNode(const glm::mat4& transform, const IMesh& mesh) {
        auto _nodeVisitor = Utils::overloaded{
            [](auto) { throw 1; },
            [&](const Contour::Mesh* mesh) { _contourProgramExecutor.QueueMesh(transform, *mesh); }
        };
        std::visit(_nodeVisitor, mesh.specify());
    }

public:
    void QueueNodes(const Node& node, const glm::mat4 transform = glm::mat4{1}) {
        const glm::mat4 totalTransform = transform * node.GetTransform();
        const IMesh* mesh = node.GetMesh();
        if (mesh != nullptr) {
            queueNode(totalTransform, *mesh);
        }

        for (const auto& childNode : node.GetChildNodes()) {
            QueueNodes(*childNode, totalTransform);
        }
    }

    void Draw(const glm::mat4& view, const glm::mat4& projection) {
        _contourProgramExecutor.Draw(view, projection);
    }
};

} // namespace Render
#pragma once

#include "Program.hpp"
#include "Node.hpp"
#include "CameraNode.hpp"
#include "MeshNode.hpp"
#include "INodeVisitor.hpp"
#include <overloaded.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Render {

class DrawingManager : private INodeVisitor {
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



    struct Camera {
        glm::mat4 projection;
        glm::mat4 view;
    } camera;

    void queueMesh(const glm::mat4& transform, const IMesh& mesh) {
        auto _nodeVisitor = Utils::overloaded{
            [](auto) { throw 1; },
            [&](const Contour::Mesh* mesh) { _contourProgramExecutor.QueueMesh(transform, *mesh); }
        };
        std::visit(_nodeVisitor, mesh.specify());
    }

    void Accept(const CameraNode& node, const glm::mat4& transform) override {
        camera.view = glm::inverse(node.GetTransform()) * glm::inverse(transform);
        camera.projection = node.GetProjectionTransform();
    };

    void Accept(const MeshNode& node, const glm::mat4& transform) override {
        const IMesh* mesh = node.GetMesh();
        if (mesh != nullptr) {
            queueMesh(node.GetTransform() * transform, *mesh);
        }
    };


public:
    inline static int count  = 0;
    void QueueNodes(const Node& node, const glm::mat4 transform = glm::mat4{1}) {
        count++;
        node.Visit(*this, transform);
        const glm::mat4 totalTransform = node.GetTransform() * transform;
        for (const auto& childNode : node.GetChildNodes()) {
            QueueNodes(*childNode, totalTransform);
        }
        count --;
    }

    void Draw() {
        _contourProgramExecutor.Draw(camera.view, camera.projection);
    }
};

} // namespace Render
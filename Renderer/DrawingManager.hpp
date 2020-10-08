#pragma once

#include "Contour/Program.hpp"
#include "Node.hpp"
#include <overloaded.hpp>

namespace Render {

class DrawingManager {
    template<class Program, class Model>
    class DrawingExecutor {
        using TransformedModel = std::pair<glm::mat4, const Model&>;
        Program _program;
        std::vector<TransformedModel> _drawingQueue;
    public:
        void QueueModel(glm::mat4 transform, const Model& model) {
            _drawingQueue.push_back({std::move(transform), model});
        }
        
        void Draw(const glm::mat4& view, const glm::mat4& projection) {
            _program.Draw(view, projection, _drawingQueue);
            _drawingQueue.clear();
        }
    };

    DrawingExecutor<Contour::Program, Contour::Model> _contourProgramExecutor;
    // more programs soon...

    void queueNode(const glm::mat4& transform, const IModel& model) {
        auto _nodeVisitor = Utils::overloaded{
            [](auto) { throw 1; },
            [&](const Contour::Model* model) { _contourProgramExecutor.QueueModel(transform, *model); }
        };
        std::visit(_nodeVisitor, model.specify());
    }

public:
    void QueueNodes(const Node& node, const glm::mat4 transform = glm::mat4{1}) {
        const glm::mat4 totalTransform = transform * node.GetTransform();
        const IModel* model = node.GetModel();
        if (model != nullptr) {
            queueNode(totalTransform, *model);
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
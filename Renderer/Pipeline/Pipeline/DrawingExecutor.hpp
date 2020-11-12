#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Render {

/**
 * This is a non-owning helper to draw elements with their proper transforms. 
 * This owns neither Mesh nor Transform so be careful. Always store transforms somewhere higher.
 * 
 * How I wish it was read is: 
 * DrawingExecutor<P, T> draws elements defined by Ts using program P.
 */
template<class Program, class Mesh>
class DrawingExecutor {
    using ProgramType = std::decay_t<Program>;
    using MeshType = std::decay_t<Mesh>;
    using TransformedMesh = std::pair<glm::mat4, const MeshType&>;

    ProgramType _program;
    std::vector<TransformedMesh> _drawingQueue;

public:
    void QueueMesh(const glm::mat4& transform, const Mesh& mesh) {
        _drawingQueue.push_back({transform, mesh});
    }
    
    void Draw(const glm::mat4& view, const glm::mat4& projection) {
        _program.Draw(view, projection, _drawingQueue);
    }

    void Clear() {
        _drawingQueue.clear();
    }

    Program& GetProgram() {
        return _program;
    }
};

} // namespace Render
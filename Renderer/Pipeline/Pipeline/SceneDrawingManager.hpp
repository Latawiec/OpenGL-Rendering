#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <Common/IdGenerator.hpp>
#include <Common/Scene.hpp>
#include <Common/NodeLink.hpp>
#include <Common/Transformed.hpp>
#include <MappingProgram/Static.hpp>
#include <MappingProgram/Animated.hpp>
#include <EdgeDetector/Program.hpp>
#include "GraphicBuffer.hpp"
#include "DrawingExecutor.hpp"

#include "TextureProgram.hpp"

namespace Render {

struct SceneDrawingManager {

SceneDrawingManager(const int windowWidth, const int windowHeight);

void Draw(const Common::Scene& scene);

private:

    void matchForQueue(const Common::Scene& scene, const Common::NodeLink& link);

    void queueContourMesh(const glm::mat4& transform, const Common::Mesh& mesh);
    void queueShadowMesh(const glm::mat4& transform, const Common::Mesh& mesh);
    void queueCamera(const glm::mat4& worldTransform, const Common::Camera& camera);

    void prepareSkins(const Common::Scene& scene);

    void drawQueues();

    struct {
        glm::mat4 viewTransform;
        glm::mat4 projectionTransform;
    } _activeCamera;

    int _width, _height;
    GraphicBuffer _deferredBuffers;

    struct {
        void Add(const glm::mat4& transform, const Common::Mesh& mesh) {
            _queue.push_back({ transform, mesh });
        }
        void Draw(const glm::mat4& view, const glm::mat4& projection) {
            _program.Draw(view, projection, _queue);    
        }
        void Clear() {
            _queue.clear();
        }
    private:
        std::vector<Common::TransformedMesh> _queue;
        MappingProgram::Static _program;
    } _contourProgramExecutor;

    struct {
        void Add(const glm::mat4& transform, const std::vector<glm::mat4>& jointTransforms, const Common::Mesh& mesh) {
            _queue.push_back({ transform, jointTransforms, mesh });
        }
        void Draw(const glm::mat4& view, const glm::mat4& projection) {
            
        }
        void Clear() {
            _queue.clear();
        }
    private:
        std::unordered_multimap<
        std::vector<TransformedAnimatedMesh> _queue;
        MappingProgram::Animated _program;
    } _animContourProgramExecutor;

    DebugUtils::TextureProgram _textureDrawProgram;
    EdgeDetector::Program _edgeProgram;

    // I'll put it here becasue I don't know how to handle it yet.
    std::unordered_map<Common::IdGenerator::Type, std::array<glm::mat4, 32>> _preparedJointTransforms;
};

} // namespace Render
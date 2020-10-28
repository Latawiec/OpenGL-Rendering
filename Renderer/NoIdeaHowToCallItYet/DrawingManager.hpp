#pragma once

#include "Contour/Program.hpp"
#include "Shadow/Program.hpp"
#include "Shadow/LightNode.hpp"
#include "Node.hpp"
#include "CameraNode.hpp"
#include "MeshNode.hpp"
#include "INodeVisitor.hpp"
#include <overloaded.hpp>
#include "GraphicBuffer.hpp"
#include "DepthBuffer.hpp"
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
        }

        void Clear() {
            _drawingQueue.clear();
        }

        Program& GetProgram() {
            return _program;
        }
    };

    DrawingExecutor<Contour::Program, Common::Mesh> _contourProgramExecutor;
    DrawingExecutor<Shadow::Program, Common::Mesh> _shadowProgramExecutor;
    // more programs soon...

    int windowWidth, windowHeight;
    GraphicBuffer deferredBuffers;
    DepthBuffer depthBuffer;

    DebugUtils::TextureProgram textureDrawProgram;

    struct Camera {
        glm::mat4 projection;
        glm::mat4 view;
    } camera;

    struct Light {
        glm::mat4 projection;
        glm::mat4 view;
    } thisSingleLightIWantToTest;

    void queueMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
        _contourProgramExecutor.QueueMesh(transform, mesh);
        _shadowProgramExecutor.QueueMesh(transform, mesh);
    }

    void Accept(const CameraNode& node, const glm::mat4& transform) override {
        camera.view = glm::inverse(node.GetTransform()) * glm::inverse(transform);
        camera.projection = node.GetProjectionTransform();
    };

    void Accept(const MeshNode& node, const glm::mat4& transform) override {
        const Common::Mesh* mesh = node.GetMesh();
        if (mesh != nullptr) {
            queueMesh(node.GetTransform() * transform, *mesh);
        }
    };

    void Accept(const LightNode& node, const glm::mat4& transform) override {
        thisSingleLightIWantToTest.view = node.GetLightView();// * glm::inverse(transform);
        thisSingleLightIWantToTest.projection = node.GetLightProjection();
    }


public:

    DrawingManager(const int windowWidth, const int windowHeight)
    : windowWidth(windowWidth),
      windowHeight(windowHeight),
      deferredBuffers(windowWidth/2, windowHeight/2),
      depthBuffer(1024, 1024) {}

    void QueueNodes(const Node& node, const glm::mat4 transform = glm::mat4{1}) {
        node.Visit(*this, transform);
        const glm::mat4 totalTransform = node.GetTransform() * transform;
        for (const auto& childNode : node.GetChildNodes()) {
            QueueNodes(*childNode, totalTransform);
        }
    }

    void Draw() {
        {
            glViewport(0, 0, 1024, 1024);
            FramebufferBase::ScopedBinding bind(depthBuffer);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT);
            glCullFace(GL_FRONT);
            _shadowProgramExecutor.Draw(thisSingleLightIWantToTest.view, thisSingleLightIWantToTest.projection);
            _contourProgramExecutor.GetProgram().SetLightSpaceMatrix(thisSingleLightIWantToTest.projection * thisSingleLightIWantToTest.view);
            _contourProgramExecutor.GetProgram().SetShadowMapTexture(depthBuffer.getTexture());
            glCullFace(GL_BACK);
        }
        {
            glViewport(0, 0, windowWidth, windowHeight);
            FramebufferBase::ScopedBinding bind(deferredBuffers);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _contourProgramExecutor.Draw(camera.view, camera.projection);
        }
        _contourProgramExecutor.Clear();
        _shadowProgramExecutor.Clear();

        // Testing
        {
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, windowWidth, windowHeight);
            //textureDrawProgram.draw(depthBuffer.getTexture());
            //edgeProgram.Draw(deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
            textureDrawProgram.draw(deferredBuffers.getTexture(GraphicBuffer::Output::Albedo));
        }
    }
};

} // namespace Render
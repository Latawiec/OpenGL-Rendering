#pragma once

#include "Contour/Program.hpp"
#include "Shadow/Program.hpp"
#include "Shadow/LightNode.hpp"
#include "ShadowMapDrawer.hpp"
#include "EdgeDetector/Program.hpp"
#include "Node.hpp"
#include "CameraNode.hpp"
#include "MeshNode.hpp"
#include "INodeVisitor.hpp"
#include <overloaded.hpp>
#include "GraphicBuffer.hpp"
#include <Common/DepthBuffer.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Render {

class DrawingManager : private INodeVisitor {


    DrawingExecutor<Contour::Program, Common::Mesh> _contourProgramExecutor;
    ShadowMapDrawer _shadowMapDrawer;
    // more programs soon...

    int windowWidth, windowHeight;
    GraphicBuffer deferredBuffers;

    DebugUtils::TextureProgram textureDrawProgram;
    // Debug...
    EdgeDetector::Program edgeProgram;
    

    struct Camera {
        glm::mat4 projection;
        glm::mat4 view;
    } camera;

    void queueMesh(const glm::mat4& transform, const Common::Mesh& mesh) {
        _contourProgramExecutor.QueueMesh(transform, mesh);
        _shadowMapDrawer.QueueMesh(transform, mesh);
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
        _shadowMapDrawer.AddLight(node.GetShadowMap(), node.GetLightView() * transform, node.GetLightProjection());
    }

public:

    DrawingManager(const int windowWidth, const int windowHeight)
    : windowWidth(windowWidth),
      windowHeight(windowHeight),
      deferredBuffers(windowWidth/2, windowHeight/2) {

      }

    void QueueNodes(const Node& node, const glm::mat4 transform = glm::mat4{1}) {
        node.Visit(*this, transform);
        const glm::mat4 totalTransform = node.GetTransform() * transform;
        for (const auto& childNode : node.GetChildNodes()) {
            QueueNodes(*childNode, totalTransform);
        }
    }

    void Draw() {
        _shadowMapDrawer.DrawShadowMaps();
        {
            const auto& shadowMapsInfo = _shadowMapDrawer.GetShadowMapsInfo();
            if (shadowMapsInfo.size() > 0) {
                // We have harcoded single light so...
                const auto& singleLightInfo = shadowMapsInfo[0];
                _contourProgramExecutor.GetProgram().SetLightSpace(singleLightInfo.viewTransform, singleLightInfo.projectionTransform);
                _contourProgramExecutor.GetProgram().SetShadowMapTexture(singleLightInfo.shadowMap.getTexture());
            }
            Common::FramebufferBase::ScopedBinding bind(deferredBuffers);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            _contourProgramExecutor.Draw(camera.view, camera.projection);
        }
        _contourProgramExecutor.Clear();
        _shadowMapDrawer.Clear();
        _shadowMapDrawer.ClearLights();

        // Testing
        {
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, windowWidth, windowHeight);
            //textureDrawProgram.draw(depthBuffer.getTexture());
            {
                edgeProgram.SetImageSize(windowWidth/2, windowHeight/2);
                edgeProgram.Draw(deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
            }
            //textureDrawProgram.draw(deferredBuffers.getTexture(GraphicBuffer::Output::EdgeInfo));
        }
    }
};

} // namespace Render
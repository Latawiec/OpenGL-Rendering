#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <id_generator.hpp>
#include <Scene/Scene.hpp>
#include <Scene/NodeLink.hpp>
#include <EdgeProgram.hpp>
#include <TextureProgram.hpp>
#include <DebugMeshProgram.hpp>
#include "BasePass/BasePassBuffer.hpp"
#include "LightingPass/LightingPassBuffer.hpp"
#include "ShadowMappingPass/ShadowMappingPassBuffer.hpp"
#include "GlobalTexturesBuffer.hpp"
#include "DepthBuffer.hpp"
#include "TransformProcessor.hpp"
#include "BasePass/BasePassPipelineManager.hpp"
#include "ShadowMappingPass/ShadowMappingPassPipelineManager.hpp"
#include "LightingPass/LightingPassPipelineManager.hpp"
#include "CombinePass/CombinePassPipelineManager.hpp"

namespace Renderer {
namespace SceneDrawing {

struct SceneDrawingManager {

SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight);

void Draw();

void SetWindowSize(const int windowWidth, const int windowHeight);
void SetResolution(const int pixelWidth, const int pixelHeight);

private:
    const Renderer::Scene::Scene& _scene;

    unsigned int _width, _height;
    unsigned int _shadowMapWidth = 2056, _shadowMapHeight = 2056;

    BasePass::BasePassBuffer _basePassBuffer;
    LightingPass::LightingPassBuffer _lightingPassBuffer;
    ShadowMappingPass::ShadowMappingPassBuffer _shadowMappingPassBuffer;
    GlobalTexturesBuffer _globalTexturesBuffer;
    
    TransformProcessor _transformProcessor;
    BasePass::BasePassPipelineManager _basePassPipelineManager;
    ShadowMappingPass::ShadowMappingPipelineManager _shadowMappingPassPipelineManager;
    LightingPass::LightingPipelineManager _lightingPassPipelineManager;
    CombinePass::CombinePipelineManager _combinePassPipelineManager;
    Scene::Base::VertexDataBase _framebufferPlane;

    ShadowMappingPass::SharedData createFittingShadowmapTransform(const Scene::Base::DirectionalLight& light, const glm::mat4 lightTransform, const Scene::Base::Camera& camera, const glm::mat4 cameraTransform);

    void prepareSkin(const Renderer::Scene::Base::Skin::IdType& skinId);
    std::unordered_map<Renderer::Scene::Base::Skin::IdType, std::array<glm::mat4, 32>> _jointTransforms;

    // Debugging, testing ... seeing if works.
    Renderer::Programs::TextureProgram _textureDrawProgram;
    Renderer::Programs::EdgeProgram _edgeProgram;

    void CombinePass();
    void LightingPass();
    void ShadowMappingPass();
    void BasePass();

    constexpr static inline glm::vec3 quadCoords[4] = {
        {-1.f, -1.f, 0.f},
        {1.f, -1.f, 0.f},
        {1.f, 1.f, 0.f},
        {-1.f, 1.f, 0.f}
    };

    constexpr static inline unsigned int indices[6] = {
        0, 1, 2, 2, 3, 0
    };
};

} // namespace SceneDrawing
} // namespace Renderer
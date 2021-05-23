#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <id_generator.hpp>
#include <Scene/Scene.hpp>
#include <Scene/NodeLink.hpp>
#include <TextureProgram.hpp>
#include <DebugMeshProgram.hpp>
#include "BasePass/BasePassBuffer.hpp"
#include "BasePass/BasePassPipelineManager.hpp"
#include "CombinePass/CombinePassPipelineManager.hpp"
#include "ContourPass/ContourPassBuffer.hpp"
#include "ContourPass/ContourPassPipelineManager.hpp"
#include "LightingPass/LightingPassBuffer.hpp"
#include "LightingPass/LightingPassPipelineManager.hpp"
#include "ShadowMappingPass/ShadowMappingPassBuffer.hpp"
#include "ShadowMappingPass/ShadowMappingPassPipelineManager.hpp"
#include "PostProcess/PostProcessBuffer.hpp"
#include "PostProcess/Blend/BlendPipelineManager.hpp"
#include "PostProcess/Blur/BlurPipelineManager.hpp"
#include "PostProcess/BrightnessFilter/BrightnessFilterPipelineManager.hpp"
#include "GlobalTexturesBuffer.hpp"
#include "DepthBuffer.hpp"
#include "TransformProcessor.hpp"

namespace Renderer {
namespace SceneDrawing {

struct SceneDrawingManager {

SceneDrawingManager(const Renderer::Scene::Scene& scene, const int windowWidth, const int windowHeight);

void Draw();

void SetWindowSize(const int windowWidth, const int windowHeight);
void SetResolution(const int pixelWidth, const int pixelHeight);
void SetActiveSceneView(unsigned int sceneViewIndex);
const glm::mat4& GetNodeWorldTransform(const Scene::Base::Node::IdType& id) const;

private:
    const Renderer::Scene::Scene& _scene;

    unsigned int _width, _height;
    unsigned int _shadowMapWidth = 2056, _shadowMapHeight = 2056;

    BasePass::BasePassBuffer _basePassBuffer;
    ContourPass::ContourPassBuffer _contourPassBuffer;
    LightingPass::LightingPassBuffer _lightingPassBuffer;
    ShadowMappingPass::ShadowMappingPassBuffer _shadowMappingPassBuffer;
    GlobalTexturesBuffer _globalTexturesBuffer;
    
    BasePass::BasePassPipelineManager _basePassPipelineManager;
    ContourPass::ContourPipelineManager _contourPassPipelineManager;
    LightingPass::LightingPipelineManager _lightingPassPipelineManager;
    ShadowMappingPass::ShadowMappingPipelineManager _shadowMappingPassPipelineManager;
    CombinePass::CombinePipelineManager _combinePassPipelineManager;

    // Bloom setup
    PostProcess::PostProcessBuffer _intermediateBloomBuffer;
    PostProcess::PostProcessBuffer _finalBloomBuffer;
    PostProcess::BrightnessFilter::BrightnessFilterPipelineManager _brightnessFilterPipelineManager;
    PostProcess::Blur::BlurPipelineManager _blurPipelineManager;
    PostProcess::Blend::BlendPipelineManager _blendPipelineManager;

    Scene::Base::VertexDataBase _framebufferPlane;
    TransformProcessor _transformProcessor;
    std::unordered_map<Renderer::Scene::Base::Skin::IdType, std::array<glm::mat4, 32>> _jointTransforms;
    uint64_t _activeSceneViewIndex = 0;

    const Scene::SceneView& getActiveSceneView() const;
    ShadowMappingPass::SharedData createFittingShadowmapTransform(const Scene::Base::DirectionalLight& light, const glm::mat4 lightTransform, const Scene::Base::Camera& camera, const glm::mat4 cameraTransform);
    void prepareSkin(const Renderer::Scene::Base::Skin::IdType& skinId);

    // Debugging, testing ... seeing if works.
    Renderer::Programs::TextureProgram _textureDrawProgram;

    void CombinePass();
    void ContourPass();
    void LightingPass();
    void ShadowMappingPass();
    void BasePass();

    void Bloom();

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
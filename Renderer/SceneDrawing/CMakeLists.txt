cmake_minimum_required(VERSION 3.9)

project(Renderer_SceneDrawing)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    src/BasePassPipelineManager.cpp
    src/BasePassBuffer.cpp
    src/BlendPipelineManager.cpp
    src/BlurPipelineManager.cpp
    src/BrightnessFilterPipelineManager.cpp
    src/CombinePassPipelineManager.cpp
    src/ContourPassBuffer.cpp
    src/ContourPassPipelineManager.cpp
    src/CubeDepthBuffer.cpp
    src/DepthBuffer.cpp
    src/FramebufferBase.cpp
    src/GlobalTexturesBuffer.cpp
    src/LightingPassBuffer.cpp
    src/LightingPassPipelineManager.cpp
    src/SceneDrawingManager.cpp
    src/ShadowMappingPipelineManager.cpp
    src/ShadowMappingPassBuffer.cpp
    src/PostProcessBuffer.cpp
    src/TransformProcessor.cpp
)

add_dependencies(${PROJECT_NAME}
    Renderer_Utils
    Renderer_Scene
    Renderer_Programs_TextureProgram
    Renderer_Programs_DebugMeshProgram
    Assets
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glm
    glad::glad
    Renderer_Utils
    Renderer_Scene
    Assets
PUBLIC
    Renderer_Programs_TextureProgram
    Renderer_Programs_DebugMeshProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    BASEPASS_MATERIAL_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/BasePass.vert.glsl"
    BASEPASS_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/BasePass.frag.glsl"

    SHADOWMAPPING_MATERIAL_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/ShadowMapping.vert.glsl"
    SHADOWMAPPING_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/ShadowMapping.frag.glsl"

    LIGHTING_MATERIAL_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Lighting.vert.glsl"
    LIGHTING_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Lighting.frag.glsl"

    COMBINE_MATERIAL_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Combine.vert.glsl"
    COMBINE_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Combine.frag.glsl"

    CONTOUR_MATERIAL_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Contour.vert.glsl"
    CONTOUR_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Contour.frag.glsl"

    FORWARD_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Forward.vert.glsl"

    BLUR_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Blur.frag.glsl"
    BLEND_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/Blend.frag.glsl"
    BRIGHTNESS_FILTER_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/src/shaders/BrightnessFilter.frag.glsl"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
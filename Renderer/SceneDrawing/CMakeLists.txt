cmake_minimum_required(VERSION 3.9)

project(Renderer_Pipeline)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    SceneDrawingManager.cpp
    TransformProcessor.cpp
    GraphicBuffer.cpp
    BasePassPipelineManager.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glm
    glad::glad
    Renderer_Common
    Renderer_MappingProgram
    Renderer_EdgeDetector
    Renderer_DebugUtils_TextureProgram
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_Common
    Renderer_MappingProgram
    Renderer_EdgeDetector
    Renderer_DebugUtils_TextureProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    BASE_MATERIAL_FRAGMENT_SOURCE_PATH="${PROJECT_SOURCE_DIR}/Material.frag.glsl"
    BASE_MATERIAL_VERTEX_SOURCE_PATH="${PROJECT_SOURCE_DIR}/Material.vert.glsl"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
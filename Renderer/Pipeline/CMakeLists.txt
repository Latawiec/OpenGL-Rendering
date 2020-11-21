cmake_minimum_required(VERSION 3.9)

project(Renderer_Pipeline)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    SceneDrawingManager.cpp
    TransformProcessor.cpp
    SkinnedMeshDrawer.cpp
    StaticMeshDrawer.cpp
    GraphicBuffer.cpp
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

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
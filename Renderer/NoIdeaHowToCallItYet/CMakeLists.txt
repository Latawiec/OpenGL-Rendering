cmake_minimum_required(VERSION 3.9)

project(Renderer)

find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    GraphicBuffer.cpp
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    Utils
    glad::glad
    glm
    Renderer_Node
    Renderer_Contour
    Renderer_Shadow
    Renderer_DebugUtils_TextureProgram
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_Node
    Renderer_Contour
    Renderer_Shadow
    Renderer_DebugUtils_TextureProgram
)

target_include_directories(${PROJECT_NAME}
PUBLIC
.
)
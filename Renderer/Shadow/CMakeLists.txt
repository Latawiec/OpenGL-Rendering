cmake_minimum_required(VERSION 3.9)

project(Renderer_Shadow)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    Program.cpp
    LightNode.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glm
    Utils
    Renderer_Common
    # Renderer_Node
    Renderer_ShaderProgram
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_Common
    # Renderer_Node
    Renderer_ShaderProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    SHADOW_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
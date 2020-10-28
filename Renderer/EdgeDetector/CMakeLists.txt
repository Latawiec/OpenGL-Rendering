cmake_minimum_required(VERSION 3.9)

project(Renderer_EdgeDetector)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    Program.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glad::glad
    glm
    Renderer_Texture
PUBLIC
    Renderer_ShaderProgram
    Renderer_Common
    Renderer_Texture
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_Common
    Renderer_Texture
    Renderer_ShaderProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    EDGE_DETECTOR_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
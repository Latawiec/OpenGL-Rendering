cmake_minimum_required(VERSION 3.9)

project(Renderer_ShadowMapping)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    Directional.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glad::glad
    glm
PUBLIC
    Renderer_ShaderProgram
    Renderer_Common
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_Common
    Renderer_ShaderProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    DIRECTIONAL_SHADOW_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    Directional
)
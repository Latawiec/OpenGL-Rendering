cmake_minimum_required(VERSION 3.9)

project(Renderer)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    Renderer.cpp
    ShaderProgram.cpp
    Texture.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glad::glad
    assimp::assimp
    glm
)

add_dependencies(${PROJECT_NAME}
    Utils
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    SHADERS_SOURCE_DIR="${PROJECT_SOURCE_DIR}/Shaders"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
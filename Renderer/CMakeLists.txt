cmake_minimum_required(VERSION 3.9)

project(Renderer)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp CONFIG REQUIRED)

add_library(${PROJECT_NAME}
# Renderer.cpp
ShaderProgram.cpp
GraphicBuffer.cpp
Texture.cpp
Node.cpp
TextureProgram.cpp
Contour/Program.cpp
Transform.cpp
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
    CONTOUR_PROGRAM="${PROJECT_SOURCE_DIR}/Contour"
    SHADERS_SOURCE_DIR="${PROJECT_SOURCE_DIR}/Shaders"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
.
)
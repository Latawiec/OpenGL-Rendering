cmake_minimum_required(VERSION 3.9)

project(Renderer)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    GraphicBuffer.cpp
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    Utils
    glad::glad
    assimp::assimp
    glm
    Renderer_DebugUtils_TextureProgram
    Renderer_Camera
    Renderer_Node
    Renderer_VertexData
    Renderer_ShaderProgram
    Renderer_Contour
    Renderer_Texture
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_DebugUtils_TextureProgram
    Renderer_Camera
    Renderer_Node
    Renderer_VertexData
    Renderer_ShaderProgram
    Renderer_Contour
    Renderer_Texture
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

add_subdirectory(Contour)
add_subdirectory(DebugUtils)
add_subdirectory(Camera)
add_subdirectory(Node)
add_subdirectory(VertexData)
add_subdirectory(ShaderProgram)
add_subdirectory(Texture)
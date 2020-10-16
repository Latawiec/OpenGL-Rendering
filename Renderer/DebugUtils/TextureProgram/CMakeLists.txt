cmake_minimum_required(VERSION 3.9)

project(Renderer_DebugUtils_TextureProgram)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    TextureProgram.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glad::glad
    Renderer_VertexData
    Renderer_ShaderProgram
)

add_dependencies(${PROJECT_NAME}
    Utils
    Renderer_VertexData
    Renderer_ShaderProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    TEXTURE_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
.
)
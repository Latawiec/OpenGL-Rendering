cmake_minimum_required(VERSION 3.9)

project(Renderer_Programs_TextureProgram)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    TextureProgram.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glad::glad
    Renderer_Utils
    Renderer_Scene
    Renderer_Programs_Base
)

add_dependencies(${PROJECT_NAME}
    Renderer_Utils
    Renderer_Scene
    Renderer_Programs_Base
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    TEXTURE_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
.
)
cmake_minimum_required(VERSION 3.9)

project(Renderer_Programs_EdgeProgram)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    EdgeProgram.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glad::glad
    glm
    Renderer_Utils
    Renderer_Scene
PUBLIC
    Renderer_Programs_Base
)

add_dependencies(${PROJECT_NAME}
    Renderer_Utils
    Renderer_Programs_Base
    Renderer_Scene
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    EDGE_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
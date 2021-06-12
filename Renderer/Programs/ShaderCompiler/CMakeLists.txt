cmake_minimum_required(VERSION 3.9)

project(Renderer_Programs_ShaderCompiler)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    src/ShaderCompiler.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glad::glad
    Renderer_Utils
)

add_dependencies(${PROJECT_NAME}
    Renderer_Utils
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
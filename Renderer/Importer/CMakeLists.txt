cmake_minimum_required(VERSION 3.9)

project(Renderer_Importer)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    Importer.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    nlohmann_json::nlohmann_json
    glm
    Renderer_Node
    Renderer_Common
    Renderer_MappingProgram
)

add_dependencies(${PROJECT_NAME}
    Renderer_Node
    Renderer_Common
    Renderer_MappingProgram
)

target_include_directories(${PROJECT_NAME}
PUBLIC
.
)
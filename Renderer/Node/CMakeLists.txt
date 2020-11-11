cmake_minimum_required(VERSION 3.9)

project(Renderer_Node)

find_package(glm REQUIRED)

add_library(${PROJECT_NAME}
    Node.cpp
    CameraNode.cpp
    MeshNode.cpp
    
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glm
    Renderer_Common
)

add_dependencies(${PROJECT_NAME}
    Renderer_Common
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
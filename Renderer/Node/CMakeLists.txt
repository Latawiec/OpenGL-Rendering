cmake_minimum_required(VERSION 3.9)

project(Renderer_Node)

find_package(glm REQUIRED)

add_library(${PROJECT_NAME}
    Node.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glm
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
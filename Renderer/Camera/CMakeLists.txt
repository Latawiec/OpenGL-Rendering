cmake_minimum_required(VERSION 3.9)

project(Renderer_Camera)

find_package(glm REQUIRED)

add_library(${PROJECT_NAME}
    BasicCamera.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glm
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
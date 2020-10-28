cmake_minimum_required(VERSION 3.9)

project(Renderer_Common)

find_package(glad REQUIRED)

add_library(${PROJECT_NAME}
    Mesh.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glad::glad
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
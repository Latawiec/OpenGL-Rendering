cmake_minimum_required(VERSION 3.9)

project(Renderer_VertexData)

find_package(glad REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME}
INTERFACE
    glad::glad
)

target_include_directories(${PROJECT_NAME}
INTERFACE
    .
)
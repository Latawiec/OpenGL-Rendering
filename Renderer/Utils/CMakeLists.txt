cmake_minimum_required(VERSION 3.9)

project(Renderer_Utils)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
INTERFACE
    .
)
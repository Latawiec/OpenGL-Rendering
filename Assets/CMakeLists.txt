cmake_minimum_required(VERSION 3.9)

project(Assets)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_definitions(${PROJECT_NAME}
INTERFACE
    ASSETS_DIR="${PROJECT_SOURCE_DIR}"
)
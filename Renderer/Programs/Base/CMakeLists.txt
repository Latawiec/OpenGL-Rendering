cmake_minimum_required(VERSION 3.9)

project(Renderer_Programs_Base)

find_package(glad REQUIRED)
find_package(glm REQUIRED)

add_library(${PROJECT_NAME}
    src/ShaderProgram.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glad::glad
    glm
    Renderer_Utils
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Renderer_Utils
)

add_dependencies(${PROJECT_NAME}
    Renderer_Utils
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
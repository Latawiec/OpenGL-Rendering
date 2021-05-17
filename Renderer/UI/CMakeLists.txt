cmake_minimum_required(VERSION 3.9)

project(Renderer_UI)

find_package(imgui REQUIRED)

add_library(${PROJECT_NAME}
    src/SceneHierarchy.cpp
    src/TransformEditor.cpp
)

add_dependencies(${PROJECT_NAME}
    Renderer_Scene
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    imgui::imgui
PUBLIC
    Renderer_Scene
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)

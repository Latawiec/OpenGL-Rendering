cmake_minimum_required(VERSION 3.9)

project(test)

find_package(glfw3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
)

add_dependencies(${PROJECT_NAME}
    Assets
    Renderer_Utils
    Renderer_SceneDrawing
    Renderer_Scene
    Renderer_Importer
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glfw
    Assets
    Renderer_Utils
    Renderer_SceneDrawing
    Renderer_Scene
    Renderer_Importer
)
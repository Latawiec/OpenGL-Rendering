cmake_minimum_required(VERSION 3.9)

project(test)

find_package(glfw3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
)

add_dependencies(${PROJECT_NAME}
    Assets
    Utils
    Renderer
    Renderer_MappingProgram
    Renderer_Camera
    Renderer_Importer
    Renderer_EdgeDetector
    Renderer_DebugUtils_TextureProgram
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Assets
    Utils
    Renderer
    Renderer_MappingProgram
    Renderer_Camera
    Renderer_Importer
    Renderer_EdgeDetector
    Renderer_DebugUtils_TextureProgram
    glfw
)
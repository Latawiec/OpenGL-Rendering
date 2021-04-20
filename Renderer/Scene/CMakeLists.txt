cmake_minimum_required(VERSION 3.9)

project(Renderer_Scene)

find_package(glad REQUIRED)

add_library(${PROJECT_NAME}
    src/Camera.cpp
    src/DirectionalLight.cpp
    src/Mesh.cpp
    src/Node.cpp
    src/NodeLink.cpp
    src/Scene.cpp
    src/Skin.cpp
    src/Texture.cpp
)

add_dependencies(${PROJECT_NAME}
    Renderer_Utils
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    glad::glad
PUBLIC
    Renderer_Utils
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
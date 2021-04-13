cmake_minimum_required(VERSION 3.9)

project(Renderer_Common)

find_package(glad REQUIRED)

add_library(${PROJECT_NAME}
    Camera.cpp
    Mesh.cpp
    Node.cpp
    NodeLink.cpp
    Scene.cpp
    Skin.cpp
    DepthBuffer.cpp
    FramebufferBase.cpp
    Texture.cpp
)

add_dependencies(${PROJECT_NAME}
    Utils
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glad::glad
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
cmake_minimum_required(VERSION 3.9)

project(Renderer_MappingProgram)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    Impl.cpp
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    Utils
    glad::glad
    glm
    # Renderer_Node
PUBLIC
    Renderer_ShaderProgram
    Renderer_Common
    
)

add_dependencies(${PROJECT_NAME}
    Utils
    # Renderer_Node
    
    Renderer_Common
    Renderer_ShaderProgram
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    MAPPING_PROGRAM_DIR="${PROJECT_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    .
)
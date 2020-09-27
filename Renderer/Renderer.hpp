#pragma once

#include "Mesh.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "GraphicBuffer.hpp"
#include "TextureProgram.hpp"

#ifndef SHADERS_SOURCE_DIR
#define SHADERS_SOURCE_DIR "INCORRECT SOURCE DIR"
#endif

namespace Render {

class Renderer {

    ShaderProgram phongToGBuffers_;
    TextureProgram textureDrawProgram_;
    GraphicBuffer deferredBuffers_;
    std::vector<const Mesh*> meshes_;
    const Camera* activeCamera_;
public:

    Renderer(const int width, const int height);
    void Draw();
    bool AddMesh(const Mesh* mesh);
    void SetCamera(const Camera* camera);

};

} // Render
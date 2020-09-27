#pragma once 

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "VertexData.hpp"
#include "ShaderProgram.hpp"
#include "read_file.hpp"

namespace Render {

class TextureProgram {
    constexpr static auto processedTextureName = std::string_view("imageTexture");

    VertexDataBase _vertexData;
    ShaderProgram _shaderProgram;

public:
    TextureProgram() {
        auto indicesVector = std::vector<unsigned int>(indices, indices + 6);
        _vertexData = VertexData<Layout::Sequential, Vec3>(indicesVector, 4, reinterpret_cast<const float*>(quadCoords));

        const auto vertexShaderCode = Utils::readFile("Shaders/Deferred/SimpleTexture/texture.vert.glsl");
	    const auto fragmentShaderCode = Utils::readFile("Shaders/Deferred/SimpleTexture/texture.frag.glsl");

        _shaderProgram = ShaderProgram(
            Shader<ShaderType::Vertex>(vertexShaderCode.c_str()),
            Shader<ShaderType::Fragment>(fragmentShaderCode.c_str())
        );
    }

    void draw(unsigned int texture) {
        _shaderProgram.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        _shaderProgram.set(processedTextureName, 0);

        auto vertexBinding = VertexDataBase::ScopedBinding(_vertexData);
        glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
    }

private:
    constexpr static inline glm::vec3 quadCoords[4] = {
        {-1.f, -1.f, 0.f},
        {1.f, -1.f, 0.f},
        {1.f, 1.f, 0.f},
        {-1.f, 1.f, 0.f}
    };

    constexpr static inline unsigned int indices[6] = {
        0, 1, 2, 2, 0, 3
    };
};

} // namespace Render
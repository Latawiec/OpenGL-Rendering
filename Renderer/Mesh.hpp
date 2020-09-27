#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string_view>
#include <vector>
#include <map>
#include <string>
#include <array>

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "VertexData.hpp"

namespace Render {

enum class TextureType : uint8_t {
    Diffuse = 0,
    Specular,
    Normal,
    SIZE
};

class Mesh {
    constexpr static auto texturesAccessPrefix = std::string_view("material.");
    constexpr static auto diffuseMapArrayName = std::string_view("diffuseTextures");
    constexpr static auto specularMapArrayName = std::string_view("specularTextures");
    constexpr static auto normalMapArrayName = std::string_view("normalTextures");

    constexpr static std::string_view mapTextureTypeToName(TextureType type) {
        switch (type) {
            case TextureType::Diffuse:
                return diffuseMapArrayName;
            case TextureType::Specular:
                return specularMapArrayName;
            case TextureType::Normal:
                return normalMapArrayName;
            default:
                throw std::runtime_error("Not valid type.");
        }
    }

    VertexDataBase _vertexData;
    std::multimap<TextureType, Texture> _textures;
public:
    
    Mesh(VertexDataBase vertexData, std::multimap<TextureType, Texture> textures)
    : _vertexData(vertexData),
      _textures(std::move(textures))
    {}

    const VertexDataBase& getVertexData() const {
        return _vertexData;
    }

    void Draw(ShaderProgram& shader) const {
        std::array<int, static_cast<size_t>(TextureType::SIZE)> textureCounters{};
        
        int textureIndex = 0;
        for (const auto& [type, texture] : _textures) {
            glActiveTexture(GL_TEXTURE0 + textureIndex);

            std::string uniformReference =
                std::string(texturesAccessPrefix) +
                std::string(mapTextureTypeToName(type)) +
                '[' +
                std::to_string(textureCounters[static_cast<int>(type)]) +
                ']';

            shader.set(uniformReference, textureIndex);
            glBindTexture(GL_TEXTURE_2D, static_cast<unsigned int>(texture));
            textureCounters[static_cast<int>(type)]++;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);

        VertexDataBase::ScopedBinding bind(_vertexData);
        glDrawElements(GL_TRIANGLES, _vertexData.vertexCount(), GL_UNSIGNED_INT, 0);
    }
};

} // namespace Render
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <always_fail.hpp>

#ifndef NDEBUG
#include <iostream>
#endif

namespace Renderer {
namespace Programs {
namespace Base {

enum class UniformType : GLenum {
Float = GL_FLOAT,
Vec2 = GL_FLOAT_VEC2,
Vec3 = GL_FLOAT_VEC3,
Vec4 = GL_FLOAT_VEC4,
Double = GL_DOUBLE,
Dvec2 = GL_DOUBLE_VEC2,
Dvec3 = GL_DOUBLE_VEC3,
Dvec4 = GL_DOUBLE_VEC4,
Int = GL_INT,
Ivec2 = GL_INT_VEC2,
Ivec3 = GL_INT_VEC3,
Ivec4 = GL_INT_VEC4,
UnsignedInt = GL_UNSIGNED_INT,
Uvec2 = GL_UNSIGNED_INT_VEC2,
Uvec3 = GL_UNSIGNED_INT_VEC3,
Uvec4 = GL_UNSIGNED_INT_VEC4,
Bool = GL_BOOL,
Bvec2 = GL_BOOL_VEC2,
Bvec3 = GL_BOOL_VEC3,
Bvec4 = GL_BOOL_VEC4,
Mat2 = GL_FLOAT_MAT2,
Mat3 = GL_FLOAT_MAT3,
Mat4 = GL_FLOAT_MAT4,
Mat2x3 = GL_FLOAT_MAT2x3,
Mat2x4 = GL_FLOAT_MAT2x4,
Mat3x2 = GL_FLOAT_MAT3x2,
Mat3x4 = GL_FLOAT_MAT3x4,
Mat4x2 = GL_FLOAT_MAT4x2,
Mat4x3 = GL_FLOAT_MAT4x3,
Dmat2 = GL_DOUBLE_MAT2,
Dmat3 = GL_DOUBLE_MAT3,
Dmat4 = GL_DOUBLE_MAT4,
Dmat2x3 = GL_DOUBLE_MAT2x3,
Dmat2x4 = GL_DOUBLE_MAT2x4,
Dmat3x2 = GL_DOUBLE_MAT3x2,
Dmat3x4 = GL_DOUBLE_MAT3x4,
Dmat4x2 = GL_DOUBLE_MAT4x2,
Dmat4x3 = GL_DOUBLE_MAT4x3,
Sampler1D = GL_SAMPLER_1D,
Sampler2D = GL_SAMPLER_2D,
Sampler3D = GL_SAMPLER_3D,
SamplerCube = GL_SAMPLER_CUBE,
Sampler1DShadow = GL_SAMPLER_1D_SHADOW,
Sampler2DShadow = GL_SAMPLER_2D_SHADOW,
Sampler1DArray = GL_SAMPLER_1D_ARRAY,
Sampler2DArray = GL_SAMPLER_2D_ARRAY,
Sampler1DArrayShadow = GL_SAMPLER_1D_ARRAY_SHADOW,
Sampler2DArrayShadow = GL_SAMPLER_2D_ARRAY_SHADOW,
Sampler2DMS = GL_SAMPLER_2D_MULTISAMPLE,
Sampler2DMSArray = GL_SAMPLER_2D_MULTISAMPLE_ARRAY,
SamplerCubeShadow = GL_SAMPLER_CUBE_SHADOW,
SamplerBuffer = GL_SAMPLER_BUFFER,
Sampler2DRect = GL_SAMPLER_2D_RECT,
Sampler2DRectShadow = GL_SAMPLER_2D_RECT_SHADOW,
Isampler1D = GL_INT_SAMPLER_1D,
Isampler2D = GL_INT_SAMPLER_2D,
Isampler3D = GL_INT_SAMPLER_3D,
IsamplerCube = GL_INT_SAMPLER_CUBE,
Isampler1DArray = GL_INT_SAMPLER_1D_ARRAY,
Isampler2DArray = GL_INT_SAMPLER_2D_ARRAY,
Isampler2DMS = GL_INT_SAMPLER_2D_MULTISAMPLE,
Isampler2DMSArray = GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
IsamplerBuffer = GL_INT_SAMPLER_BUFFER,
Isampler2DRect = GL_INT_SAMPLER_2D_RECT,
Usampler1D = GL_UNSIGNED_INT_SAMPLER_1D,
Usampler2D = GL_UNSIGNED_INT_SAMPLER_2D,
Usampler3D = GL_UNSIGNED_INT_SAMPLER_3D,
UsamplerCube = GL_UNSIGNED_INT_SAMPLER_CUBE,
Usampler1DArray = GL_UNSIGNED_INT_SAMPLER_1D_ARRAY,
Usampler2DArray = GL_UNSIGNED_INT_SAMPLER_2D_ARRAY,
Usampler2DMS = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE,
Usampler2DMSArray = GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY,
UsamplerBuffer = GL_UNSIGNED_INT_SAMPLER_BUFFER,
Usampler2DRect = GL_UNSIGNED_INT_SAMPLER_2D_RECT,
Image1D = GL_IMAGE_1D,
Image2D = GL_IMAGE_2D,
Image3D = GL_IMAGE_3D,
Image2DRect = GL_IMAGE_2D_RECT,
ImageCube = GL_IMAGE_CUBE,
ImageBuffer = GL_IMAGE_BUFFER,
Image1DArray = GL_IMAGE_1D_ARRAY,
Image2DArray = GL_IMAGE_2D_ARRAY,
Image2DMS = GL_IMAGE_2D_MULTISAMPLE,
Image2DMSArray = GL_IMAGE_2D_MULTISAMPLE_ARRAY,
Iimage1D = GL_INT_IMAGE_1D,
Iimage2D = GL_INT_IMAGE_2D,
Iimage3D = GL_INT_IMAGE_3D,
Iimage2DRect = GL_INT_IMAGE_2D_RECT,
IimageCube = GL_INT_IMAGE_CUBE,
IimageBuffer = GL_INT_IMAGE_BUFFER,
Iimage1DArray = GL_INT_IMAGE_1D_ARRAY,
Iimage2DArray = GL_INT_IMAGE_2D_ARRAY,
Iimage2DMS = GL_INT_IMAGE_2D_MULTISAMPLE,
Iimage2DMSArray = GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
Uimage1D = GL_UNSIGNED_INT_IMAGE_1D,
Uimage2D = GL_UNSIGNED_INT_IMAGE_2D,
Uimage3D = GL_UNSIGNED_INT_IMAGE_3D,
Uimage2DRect = GL_UNSIGNED_INT_IMAGE_2D_RECT,
UimageCube = GL_UNSIGNED_INT_IMAGE_CUBE,
UimageBuffer = GL_UNSIGNED_INT_IMAGE_BUFFER,
Uimage1DArray = GL_UNSIGNED_INT_IMAGE_1D_ARRAY,
Uimage2DArray = GL_UNSIGNED_INT_IMAGE_2D_ARRAY,
Uimage2DMS = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE,
Uimage2DMSArray = GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY,
Atomic_uint = GL_UNSIGNED_INT_ATOMIC_COUNTER
};

template<UniformType Type>
struct UniformValue {
    
    UniformValue(GLuint shaderProgram, std::string_view uniformName) {
        _shaderProgram = shaderProgram;
        _uniformLocation = glGetUniformLocation(_shaderProgram, uniformName.data());
        #ifndef NDEBUG
        {
            if (_uniformLocation == -1) {
                std::cerr << "Failed to retrieve uniform \"" << uniformName << "\" from program " << shaderProgram << ". Could be optimized out?\n";
                return;
            } 

            GLuint uniformIndex;
            const char* uniformNames[] { uniformName.data() };
            glGetUniformIndices(_shaderProgram, 1, uniformNames, &uniformIndex);

            if (uniformIndex == GL_INVALID_INDEX) {
                // Could be element from array as someArray[i]. We can check that in Debug.
                if (const auto indexStart = uniformName.find('[')) {
                    const auto arrayName = std::string(uniformName.substr(0, indexStart)); // String because it has to be null-terminated.
                    const char* arrayNames[] = { arrayName.c_str() };
                    glGetUniformIndices(_shaderProgram, 1, arrayNames, &uniformIndex);
                }
            }

            GLint uniformSize;
            GLenum uniformType;
            glGetActiveUniform(_shaderProgram, uniformIndex, 0, nullptr, &uniformSize, &uniformType, nullptr);
            if (static_cast<GLenum>(Type) != uniformType) {
                std::cerr << "Retreived uniform \"" << uniformName << "\" is not of expected type " << uniformType << ". Should be " << static_cast<GLenum>(Type) << ".\n";
                throw std::runtime_error("Retrieved uniform of type different than expected.\n");
            }
        }
        #endif
    }

    template<typename T>
    void Set(const T& value) const {

        // Uniform could be optimized out if unused. We handle this!
        if (_uniformLocation == -1) {
            return;
        }

        // Evaluate
        if constexpr (UniformType::Bool == Type) {
            if constexpr (std::is_same_v<bool, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, (int)value); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Int == Type) {
            if constexpr (std::is_same_v<std::int16_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, value); else
            if constexpr (std::is_same_v<std::int32_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, value); else
            if constexpr (std::is_same_v<std::int64_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, value); else
            if constexpr (std::is_same_v<std::uint16_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::uint32_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::uint64_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::UnsignedInt == Type) {
            if constexpr (std::is_same_v<std::uint16_t, T>) return glProgramUniform1ui(_shaderProgram, _uniformLocation, value); else
            if constexpr (std::is_same_v<std::uint32_t, T>) return glProgramUniform1ui(_shaderProgram, _uniformLocation, value); else
            if constexpr (std::is_same_v<std::uint64_t, T>) return glProgramUniform1ui(_shaderProgram, _uniformLocation, value); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Float == Type) {
            if constexpr (std::is_same_v<float, T>) return glProgramUniform1f(_shaderProgram, _uniformLocation, value); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Vec2 == Type) {
            if constexpr (std::is_same_v<glm::vec2, T>) return glProgramUniform2fv(_shaderProgram, _uniformLocation, 1, glm::value_ptr(value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Vec3 == Type) {
            if constexpr (std::is_same_v<glm::vec3, T>) return glProgramUniform3fv(_shaderProgram, _uniformLocation, 1, glm::value_ptr(value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Vec4 == Type) {
            if constexpr (std::is_same_v<glm::vec4, T>) return glProgramUniform4fv(_shaderProgram, _uniformLocation, 1, glm::value_ptr(value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Mat4 == Type) {
            if constexpr (std::is_same_v<glm::mat4, T>) return glProgramUniformMatrix4fv(_shaderProgram, _uniformLocation, 1, GL_FALSE, glm::value_ptr(value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Sampler2D == Type) {
            if constexpr (std::is_same_v<std::int16_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::int32_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::int64_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::uint16_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::uint32_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            if constexpr (std::is_same_v<std::uint64_t, T>) return glProgramUniform1i(_shaderProgram, _uniformLocation, GLint(value)); else
            evaluationFailed<T>();
        }
        else
        {
            evaluationFailed<T>();
        }

    }

private:
    template<typename T>
    static constexpr void evaluationFailed() {
        static_assert(Utils::always_fail<Type>::value, "Cannot set this value type to given Uniform value. Maybe it needs new Set definition?");
    }

    GLuint _shaderProgram;
    GLint _uniformLocation;
};




template<UniformType Type, unsigned int Size>
struct UniformArray {

    UniformArray(GLuint shaderProgram, std::string_view uniformName) {
        _shaderProgram = shaderProgram;
        _uniformLocation = glGetUniformLocation(_shaderProgram, uniformName.data());
        #ifndef NDEBUG
        {
            if (_uniformLocation == -1) {
                std::cerr << "Failed to retrieve uniform \"" << uniformName << "\" from program " << shaderProgram << ". Could be optimized out?\n";
                return;
            } 

            GLuint uniformIndex;
            const char* uniformNames[] { uniformName.data() };
            glGetUniformIndices(_shaderProgram, 1, uniformNames, &uniformIndex);

            GLchar name [256];
            GLint uniformSize;
            GLenum uniformType;
            glGetActiveUniform(_shaderProgram, uniformIndex, 255, NULL, &uniformSize, &uniformType, name);
            if (static_cast<GLenum>(Type) != uniformType) {
                std::cerr << "Retreived uniform \"" << uniformName << "\" is not of expected type " << uniformType << ". Should be " << static_cast<GLenum>(Type) << ".\n";
                throw std::exception("Retreived uniform of different type.");
            }

            if (uniformSize != Size) {
                std::cerr << "Retreived uniform \"" << uniformName << "\" is not of expected size " << uniformSize << ". Should be " << Size << ".\n";
                throw std::exception("Retreived uniform of different size.");
            }
        }
        #endif
    }

    template<typename T>
    void Set(const T* value, const unsigned int size) const {
        
        // Uniform could be optimized out if unused. We handle this!
        if (_uniformLocation == -1) {
            return;
        }

        // Evaluate
        if constexpr (UniformType::Mat4 == Type) {
            if constexpr (std::is_same_v<glm::mat4, T>) return glProgramUniformMatrix4fv(_shaderProgram, _uniformLocation, size, GL_FALSE, glm::value_ptr(*value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Sampler2D == Type) {
            if constexpr (std::is_same_v<GLint, T>) return glProgramUniform1iv(_shaderProgram, _uniformLocation, size, value); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Vec2 == Type) {
            if constexpr (std::is_same_v<glm::vec2, T>) return glProgramUniform2fv(_shaderProgram, _uniformLocation, size, glm::value_ptr(*value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Vec3 == Type) {
            if constexpr (std::is_same_v<glm::vec3, T>) return glProgramUniform3fv(_shaderProgram, _uniformLocation, size, glm::value_ptr(*value)); else
            evaluationFailed<T>();
        }
        else
        if constexpr (UniformType::Vec4 == Type) {
            if constexpr (std::is_same_v<glm::vec4, T>) return glProgramUniform4fv(_shaderProgram, _uniformLocation, size, glm::value_ptr(*value)); else
            evaluationFailed<T>();
        }
        else
        {
            evaluationFailed<T>();
        }
    }

private:
    template<typename T>
    static constexpr void evaluationFailed() {
        static_assert(Utils::always_fail<Type>::value, "Cannot set this value type to given Uniform value. Maybe it needs new Set definition?");
    }

    GLuint _shaderProgram;
    GLint _uniformLocation;
};



} // namespace Base
} // namespace Programs
} // namespace Renderer

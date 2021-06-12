#pragma once

#include <glad/glad.h>
#include <string_view>
#include <vector>
#include <string>

namespace Renderer {
namespace Programs {
namespace ShaderCompiler {

static constexpr std::string_view VersionFlag = "#version 410 core\n";

enum ShaderType {
    Vertex,
    Fragment,
    Geometry
};

struct ShaderData {

    ShaderData(const ShaderType type, const std::string& sourceFilePath);
    void AddFlag(const std::string& flag);
    void AddFlag(const std::string_view& flag);

    ShaderType GetType() const;
    const std::vector<std::string_view>& GetStaticFlags() const;
    const std::vector<std::string>& GetDynamicFlags() const;
    const std::string& GetShaderCode() const;

private:
    std::vector<std::string_view> _staticFlags;
    std::vector<std::string> _dynamicFlags;
    std::string _shaderCode;
    ShaderType _type;
};

GLuint Compile(const ShaderData& data);

} // namespace ShaderCompiler
} // namespace Programs
} // namespace Renderer
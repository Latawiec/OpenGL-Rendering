#include "Renderer.hpp"
#include "read_file.hpp"


#ifndef SHADERS_SOURCE_DIR
#define SHADERS_SOURCE_DIR "INCORRECT SOURCE DIR"
#endif

namespace Render {

Renderer::Renderer(const int width, const int height)
: deferredBuffers_(width, height)
, phongToGBuffers_(
    Shader<ShaderType::Vertex>(Utils::readFile(SHADERS_SOURCE_DIR "/Deferred/Phong/" "phong.vert.glsl").c_str()),
    Shader<ShaderType::Fragment>(Utils::readFile(SHADERS_SOURCE_DIR "/Deferred/Phong/" "phong.frag.glsl").c_str())
)
{
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
}

void Renderer::Draw() {

    // temporary... hopefully
    constexpr static auto modelName = std::string_view("model");
    constexpr static auto viewName = std::string_view("view");
    constexpr static auto projectionName = std::string_view("projection");

    if (activeCamera_ == nullptr)
        return;

    {
        glEnable(GL_DEPTH_TEST);
        FramebufferBase::ScopedBinding bind(deferredBuffers_);
        phongToGBuffers_.use();
        phongToGBuffers_.set(modelName, glm::mat4(1.0f));
        phongToGBuffers_.set(viewName, activeCamera_->getViewTransform());
        phongToGBuffers_.set(projectionName, activeCamera_->getProjectionTransform());

        for(auto mesh : meshes_) {
            mesh->Draw(phongToGBuffers_);
        }
    }
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    textureDrawProgram_.draw(deferredBuffers_.getTexture(GraphicBuffer::Output::Albedo));
}

bool Renderer::AddMesh(const Mesh* mesh) {
    meshes_.push_back(mesh);
    return true;
}

void Renderer::SetCamera(const Camera* camera) {
    activeCamera_ = camera;
}

} // namespace Render
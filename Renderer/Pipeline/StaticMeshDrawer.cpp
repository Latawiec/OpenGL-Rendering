#include "Pipeline/StaticMeshDrawer.hpp"

namespace Render {
namespace Pipeline {

StaticMeshDrawer::StaticMeshDrawer(const Common::Scene& scene) 
: _scene(scene)
{}

void StaticMeshDrawer::Draw(const glm::mat4& view,
    const glm::mat4& proj,
    const std::unordered_map<Common::Scene::NodeIdType, glm::mat4>& nodeGlobalTransforms)
{
    const auto binding = _program.Bind();
    
    _program.SetView(view);
    _program.SetProjection(proj);

    for (const auto&[nodeId, meshId] : _scene.GetStaticMeshNodes()) {
            
        const auto& model = nodeGlobalTransforms.at(nodeId);
        _program.SetModel(model);

        const auto& mesh = _scene.GetMesh(meshId);
        _program.Draw(mesh);
    }
}

} // Pipeline
} // Render
#pragma once

#ifndef NDEBUG
#include <iostream>
#endif

#include "Base/Camera.hpp"
#include "Base/DirectionalLight.hpp"
#include "Base/PointLight.hpp"
#include "Base/SpotLight.hpp"
#include "Base/Material.hpp"
#include "Base/Mesh.hpp"
#include "Base/Node.hpp"
#include "Base/Skin.hpp"
#include "Base/Texture.hpp"
#include "NodeLink.hpp"
#include "SceneObject.hpp"
#include "SceneView.hpp"
#include "SceneLight.hpp"

#include <id_generator.hpp>

#include <array>
#include <unordered_map>

namespace Renderer {
namespace Scene {

struct Scene {

    Scene();

    Base::Node::IdType AddNode(Base::Node&& node);
    Base::Node& GetNode(const Base::Node::IdType& id);
    const Base::Node& GetNode(const Base::Node::IdType& id) const;

    Base::Mesh::IdType AddMesh(Base::Mesh&& mesh);
    Base::Mesh& GetMesh(const Base::Mesh::IdType& id);
    const Base::Mesh& GetMesh(const Base::Mesh::IdType& id) const;

    Base::Texture::IdType AddTexture(Base::Texture&& texture);
    Base::Texture& GetTexture(const Base::Texture::IdType& id);
    const Base::Texture& GetTexture(const Base::Texture::IdType& id) const;

    Base::Material::IdType AddMaterial(Base::Material&& material);
    Base::Material& GetMaterial(const Base::Material::IdType& id);
    const Base::Material& GetMaterial(const Base::Material::IdType& id) const;

    Base::Camera::IdType AddCamera(Base::Camera&& camera);
    Base::Camera& GetCamera(const Base::Camera::IdType& id);
    const Base::Camera& GetCamera(const Base::Camera::IdType& id) const;

    Base::Skin::IdType AddSkin(Base::Skin&& skin);
    Base::Skin& GetSkin(const Base::Skin::IdType& id);
    const Base::Skin& GetSkin(const Base::Skin::IdType& id) const;
    const std::unordered_map<Base::Skin::IdType, Base::Skin>& GetSkins() const;

    Base::DirectionalLight::IdType AddDirectionalLight(Base::DirectionalLight&& light);
    Base::DirectionalLight& GetDirectionalLight(const Base::DirectionalLight::IdType id);
    const Base::DirectionalLight& GetDirectionalLight(const Base::DirectionalLight::IdType id) const;
    const std::unordered_map<Base::DirectionalLight::IdType, Base::DirectionalLight>& GetDirectionalLights() const;

    Base::PointLight::IdType AddPointLight(Base::PointLight&& light);
    Base::PointLight& GetPointLight(const Base::PointLight::IdType id);
    const Base::PointLight& GetPointLight(const Base::PointLight::IdType id) const;
    const std::unordered_map<Base::PointLight::IdType, Base::PointLight>& GetPointLights() const;

    Base::SpotLight::IdType AddSpotLight(Base::SpotLight&& light);
    Base::SpotLight& GetSpotLight(const Base::SpotLight::IdType id);
    const Base::SpotLight& GetSpotLight(const Base::SpotLight::IdType id) const;
    const std::unordered_map<Base::SpotLight::IdType, Base::SpotLight>& GetSpotLights() const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    void AddSceneObject(SceneObject sceneObject);
    const std::vector<SceneObject>& GetSceneObjects() const;

    void AddSceneView(SceneView sceneView);
    const std::vector<SceneView>& GetSceneViews() const;

    void AddSceneLight(SceneLight sceneLight);
    const std::vector<SceneLight>& GetSceneLights() const;

private:
    Utils::IdGenerator<Base::Node::IdType> _nodeIdGenerator;
    Utils::IdGenerator<Base::Mesh::IdType> _meshIdGenerator;
    Utils::IdGenerator<Base::Texture::IdType> _textureIdGenerator;
    Utils::IdGenerator<Base::Material::IdType> _materialIdGenerator;
    Utils::IdGenerator<Base::Camera::IdType> _cameraIdGenerator;
    Utils::IdGenerator<Base::Skin::IdType> _skinIdGenerator;
    Utils::IdGenerator<Base::DirectionalLight::IdType> _directionalLightIdGenerator;
    Utils::IdGenerator<Base::PointLight::IdType> _pointLightIdGenerator;
    Utils::IdGenerator<Base::SpotLight::IdType> _spotLightIdGenerator;

    // Raw data
    std::unordered_map<Base::Node::IdType, Base::Node> _nodes;
    std::unordered_map<Base::Mesh::IdType, Base::Mesh> _meshes;
    std::unordered_map<Base::Texture::IdType, Base::Texture> _textures;
    std::unordered_map<Base::Material::IdType, Base::Material> _materials;
    std::unordered_map<Base::Camera::IdType, Base::Camera> _cameras;
    std::unordered_map<Base::Skin::IdType, Base::Skin> _skins;
    std::unordered_map<Base::DirectionalLight::IdType, Base::DirectionalLight> _directionalLights;
    std::unordered_map<Base::PointLight::IdType, Base::PointLight> _pointLights;
    std::unordered_map<Base::SpotLight::IdType, Base::SpotLight> _spotLights;

    // Data connections
    std::vector<SceneObject> _sceneObjects;
    std::vector<SceneView> _sceneViews;
    std::vector<SceneLight> _sceneLights;

    NodeLink _sceneRoot;

#ifndef NDEBUG
public:
    static void printNodeLink(std::ostream& stream, Scene& scene, const NodeLink& nodeLink, int level = 0) {
        stream << std::string(level, '-') << scene._nodes[nodeLink.GetNode()].GetName() << '\n';
        for (const auto& childNodeLink : nodeLink.GetChildren()) {
            printNodeLink(stream, scene, childNodeLink, level + 1);
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, Scene& scene);
#endif
};

} // namespace Scene
} // namespace Renderer
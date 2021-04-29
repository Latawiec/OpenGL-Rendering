#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Scene/Scene.hpp"

namespace tinygltf {
struct Model;
}

namespace Renderer {
namespace Importer {

class Importer {
public:
    bool importGltf(const std::string& filename, Scene::Scene& scene);

private:
    using gltfId = std::size_t; // basically index in array...
    using SceneId = std::uint64_t;

    struct {
        std::vector<SceneId> convertedNodes;
        std::unordered_map<gltfId, gltfId> cameraToOrientationNode;
        std::unordered_map<gltfId, gltfId> directionalLightToOrientationNode;
    } nodeConversionData;

    struct {
        std::vector<SceneId> convertedMeshes;
    } meshConversionData;

    struct {
        std::vector<SceneId> convertedCameras;
    } cameraConversionData;

    struct {
        std::vector<SceneId> convertedImages;
    } texturesConversionData;

    struct {
        std::vector<SceneId> convertedMaterials;
    } materialsConversionData;

    struct {
        std::unordered_map<gltfId, SceneId> convertedDirectionalLights;
    } lightsConversionData;

    struct {
        std::unordered_map<gltfId, std::vector<Scene::Base::Skin::Bone>> skinsConvertedBones;
        std::unordered_map<gltfId, std::unordered_set<gltfId>> skinsBoneNodes;
        std::unordered_map<gltfId, Scene::Base::Skin::BoneLink> skinsHierarchy;
        std::vector<SceneId> convertedSkins;
    } skinConversionData;

    void convertNodes(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    void convertMeshes(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    void convertCameras(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    void convertSkins(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    void convertTextures(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    void convertMaterials(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    void convertLights(Scene::Scene& scene, const tinygltf::Model& gltfModel);
    Scene::Base::Skin::BoneLink traverseSkinNodes(std::unordered_map<gltfId, size_t>& skinBoneNodesMissing, const tinygltf::Model& gltfModel, const size_t nodeIndex);
    Scene::NodeLink traverseNodes(Scene::Scene& scene, const tinygltf::Model& gltfModel, const size_t glftNodeId);

    void clearCache();
};

} // namespace Importer
} // namespace Renderer
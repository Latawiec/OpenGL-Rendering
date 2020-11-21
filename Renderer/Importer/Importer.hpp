#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include "Common/Scene.hpp"

namespace tinygltf {
struct Model;
}

namespace Render {

class Importer {
public:
    bool importGltf(const std::string& filename, Common::Scene& scene);

private:
    using gltfId = std::size_t; // basically index in array...
    using SceneId = std::uint64_t;

    struct {
        std::vector<SceneId> convertedNodes;
        std::unordered_map<gltfId, gltfId> cameraToOrientationNode;
    } nodeConversionData;

    struct {
        std::vector<SceneId> convertedMeshes;
    } meshConversionData;

    struct {
        std::vector<SceneId> convertedCameras;
    } cameraConversionData;

    struct {
        std::unordered_map<gltfId, std::vector<Common::Skin::Bone>> skinsConvertedBones;
        std::unordered_map<gltfId, std::unordered_set<gltfId>> skinsBoneNodes;
        std::unordered_map<gltfId, Common::Skin::BoneLink> skinsHierarchy;
        std::vector<SceneId> convertedSkins;
    } skinConversionData;

    void convertNodes(Common::Scene& scene, const tinygltf::Model& gltfModel);
    void convertMeshes(Common::Scene& scene, const tinygltf::Model& gltfModel);
    void convertCameras(Common::Scene& scene, const tinygltf::Model& gltfModel);
    void convertSkins(Common::Scene& scene, const tinygltf::Model& gltfModel);
    Common::Skin::BoneLink traverseSkinNodes(std::unordered_map<gltfId, size_t>& skinBoneNodesMissing, const tinygltf::Model& gltfModel, const size_t nodeIndex);
    Common::NodeLink traverseNodes(Common::Scene& scene, const tinygltf::Model& gltfModel, const size_t glftNodeId);

    void clearCache();
};

} // namespace Render
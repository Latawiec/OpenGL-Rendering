
#include "Importer.hpp"
#include <nlohmann/json.hpp>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_JSON
#include <tiny_gltf.h>
#include <glad/glad.h>

#include "Common/VertexData.hpp"
#include "Common/VertexAttribute.hpp"
#include "Common/Mesh.hpp"
#include "Common/NodeLink.hpp"
#include "Common/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <array>

#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Render {
using namespace Common;
namespace /*anonymous*/ {
    using gltfId = std::size_t; // basically index in array...
    using SceneId = std::uint64_t;
    static const std::string ProgramType = "ProgramType";
    static const std::string ContourProgramType = "Contour";

    static const std::string PositionAttribute = "POSITION";
    static const std::string NormalsAttribute = "NORMAL";
    static const std::string UvAttribute = "TEXCOORD_0";
    static const std::string EdgeColourAttribute = "COLOR_0";
    static const std::string JointsAttribute = "JOINTS_0";
    static const std::string JointsWeightsAttribute = "WEIGHTS_0";

    Common::Mesh processContourMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // I assume single primitve.
        const auto primitive = mesh.primitives[0];

        const auto& positionAccessorId = primitive.attributes.at(PositionAttribute);
        const auto& normalAccessorId = primitive.attributes.at(NormalsAttribute);
        const auto& uvAccessorId = primitive.attributes.at(UvAttribute);
        const auto& edgeInfoAccessorId = primitive.attributes.at(EdgeColourAttribute);

        // const auto& jointsAccessorId = primitive.attributes.at(JointsAttribute);
        // const auto& jointsWeightsAccessorId = primitive.attributes.at(JointsWeightsAttribute);

        const auto& indicesAccessorId = primitive.indices;
        
        const std::map<Common::VertexAttribute, const int&> locationToAccessorMap {
            { Common::VertexAttribute::POSITION, positionAccessorId },
            { Common::VertexAttribute::NORMAL, normalAccessorId },
            { Common::VertexAttribute::UV_MAP, uvAccessorId },
            { Common::VertexAttribute::EDGE_INFO, edgeInfoAccessorId }
            // { Common::VertexAttribute::JOINT, jointsAccessorId },
            // { Common::VertexAttribute::JOINT_WEIGHT, jointsWeightsAccessorId }
        };

        for (const auto&[attrLocation, accessorId] : locationToAccessorMap) {
            const auto& accessor = model.accessors[accessorId]; 
            const auto& bufferView = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[bufferView.buffer];
            const int attrSize = accessor.type % 32;
            const int byteStride = accessor.ByteStride(bufferView);

            GLuint glBuffer;
            glGenBuffers(1, &glBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, glBuffer);
            glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, 
                         &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
            glEnableVertexAttribArray(attrLocation);
            glVertexAttribPointer(attrLocation, attrSize, accessor.componentType,
                                  accessor.normalized ? GL_TRUE : GL_FALSE,
                                  byteStride, BUFFER_OFFSET(accessor.byteOffset));
            // glDeleteBuffers(1, &glBuffer); Why the hell can't I delete these? :(
        }

        // Indices
        const auto& indicesAccessor = model.accessors[indicesAccessorId];
        const auto& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
        const auto& indicesBuffer = model.buffers[indicesBufferView.buffer];
        const int indicesSize =  indicesAccessor.type % 32;

        GLuint glBuffer;
        glGenBuffers(1, &glBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBufferView.byteLength,
                     &indicesBuffer.data.at(0) + indicesBufferView.byteOffset, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        // glDeleteBuffers(1, &glBuffer); Why the hell can't I delete these? :(

        // if (!model.skins.empty()) {
        //     const auto& inverseBindMatricesAccessorId = model.skins[0].inverseBindMatrices;
        //     const auto& inverseBindMatricesAccessor = model.accessors[inverseBindMatricesAccessorId];
        //     const auto& inverseBindMatricesBufferView = model.bufferViews[inverseBindMatricesAccessor.bufferView];
        //     const auto& inverseBindMatricesBuffer = model.buffers[inverseBindMatricesBufferView.buffer];
        //     const auto inverseBindMatricesCount = inverseBindMatricesAccessor.count;
        //     const auto inverseBindMatricesStride = inverseBindMatricesAccessor.ByteStride(inverseBindMatricesBufferView);

        //     std::vector<glm::mat4> inverseBindMatrices(inverseBindMatricesCount);
        //     std::memcpy(inverseBindMatrices.data(),
        //         &inverseBindMatricesBuffer.data.at(0) + inverseBindMatricesBufferView.byteOffset,
        //         inverseBindMatricesBufferView.byteLength
        //     );
        //     std::cout << "Bind matrices!\n"; 
        // }

        return Common::Mesh{ VertexDataBase(VAO, indicesAccessor.count) };
    }
}

Common::Mesh processMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {

    const auto& material = model.materials[mesh.primitives[0].material];
    const auto& properties = material.extras;

    if (properties.Has(ProgramType) && properties.Get(ProgramType).IsString()) {
        const auto& materialType = properties.Get(ProgramType).Get<std::string>();
        if (materialType == ContourProgramType) {
            return processContourMesh(model, mesh);
        }
    }

    return {};
}

Common::Camera processCamera(const glm::mat4& cameraOrientation, const tinygltf::Camera& camera) {
    return Common::Camera(cameraOrientation, camera.perspective.yfov, 800.f/600.f);
}

glm::mat4 processNodeTransform(const tinygltf::Node& node) {
    const auto scaleVec = node.scale.size() == 0 ? glm::vec3(1.0) : glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
    const auto rotationQuat = node.rotation.size() == 0 ? glm::quat(1.0, 0.0, 0.0, 0.0) : glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
    const auto translationVec = node.translation.size() == 0 ? glm::vec3(0.0) : glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
    
    const auto scaleMatrix = glm::scale(glm::mat4(1.0), scaleVec);
    const auto rotateMatrix = glm::mat4_cast(rotationQuat);
    const auto translateMatrix = glm::translate(glm::mat4(1.0), translationVec);

    return translateMatrix * rotateMatrix * scaleMatrix;
}

Common::NodeLink processNode(
    const tinygltf::Model& model,
    const gltfId nodeIndex,
    Common::Scene& scene,
    std::map<gltfId, SceneId>& meshes,
    std::map<gltfId, SceneId>& cameras,
    std::map<gltfId, SceneId>& skins,
    std::map<gltfId, SceneId>& nodes)
{
    const auto& node = model.nodes[nodeIndex];
    const auto nodeTransform = processNodeTransform(node);
    auto sceneNode = Common::Node{nodeTransform};
    #ifndef NDEBUG
    sceneNode.SetName(node.name);
    #endif
    const auto nodeId = scene.AddNode(std::move(sceneNode));
    nodes.insert({ nodeIndex, nodeId });
    uint16_t nodeLinkProperties = 0x0;
    Common::NodeLink result(nodeId);

    if (node.camera != -1) {
        if (!cameras.contains(node.camera)) {
            auto processedCamera = processCamera(nodeTransform, model.cameras[node.camera]);
            const auto cameraId = scene.AddCamera(std::move(processedCamera));
            cameras.insert({ node.camera, cameraId });
        }
        result.SetCamera(cameras[node.camera]);
        nodeLinkProperties |= NodeLink::Properties::CAMERA;
    }

    if (node.mesh != -1) {
        if (!meshes.contains(node.mesh)) {
            auto processedMesh = processMesh(model, model.meshes[node.mesh]);
            const auto meshId = scene.AddMesh(std::move(processedMesh));
            meshes.insert({ node.mesh, meshId });
        }
        result.SetMesh(meshes[node.mesh]);
        nodeLinkProperties |= NodeLink::Properties::CONTOUR_MESH;
        nodeLinkProperties |= NodeLink::Properties::CASTS_SHADOW;
    }

    if (node.skin != -1) {
        nodeLinkProperties |= NodeLink::Properties::SKINNED;
    }

    result.SetProperties(nodeLinkProperties);

    for (std::size_t index = 0; index < node.children.size(); index++) {
        result.AddChild(
            processNode(model, node.children[index], scene, meshes, cameras, skins, nodes)
        );
    }

    return result;
}

bool Importer::importGltf(const std::string& filename, Common::Scene& scene) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
    }

    std::map<gltfId, SceneId> meshes;
    std::map<gltfId, SceneId> cameras;
    std::map<gltfId, SceneId> skins;
    std::map<gltfId, SceneId> nodes;

    const auto sceneRootNodeId = scene.AddNode({});
    Common::NodeLink sceneRootNodeLink{ sceneRootNodeId, 0x0 };

    const tinygltf::Scene &gltfScene = model.scenes[model.defaultScene];

    for (size_t i = 0; i < gltfScene.nodes.size(); ++i) {
        assert((gltfScene.nodes[i] >= 0) && (gltfScene.nodes[i] < model.nodes.size()));
        sceneRootNodeLink.AddChild(
            processNode(model, gltfScene.nodes[i], scene,meshes, cameras, skins, nodes)
        );
    }

    scene.AddNodeHierarchy(std::move(sceneRootNodeLink));

    return true;
}

} // namespace Render
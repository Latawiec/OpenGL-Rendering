
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
#include "MeshNode.hpp"
#include "CameraNode.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <array>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Render {
namespace /*anonymous*/ {
    static const std::string ProgramType = "ProgramType";
    static const std::string ContourProgramType = "Contour";

    static const std::string PositionAttribute = "POSITION";
    static const std::string NormalsAttribute = "NORMAL";
    static const std::string UvAttribute = "TEXCOORD_0";
    static const std::string EdgeColourAttribute = "COLOR_0";

    std::unique_ptr<Node> processContourMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // I assume single primitve.
        const auto primitive = mesh.primitives[0];

        const auto& positionAccessorId = primitive.attributes.at(PositionAttribute);
        const auto& normalAccessorId = primitive.attributes.at(NormalsAttribute);
        const auto& uvAccessorId = primitive.attributes.at(UvAttribute);
        const auto& edgeInfoAccessorId = primitive.attributes.at(EdgeColourAttribute);

        const auto& indicesAccessorId = primitive.indices;
        
        const std::map<Common::VertexAttribute, const int&> locationToAccessorMap {
            { Common::VertexAttribute::POSITION, positionAccessorId },
            { Common::VertexAttribute::NORMAL, normalAccessorId },
            { Common::VertexAttribute::UV_MAP, uvAccessorId },
            { Common::VertexAttribute::EDGE_INFO, edgeInfoAccessorId }
        };

        for (const auto&[attrLocation, accessorId] : locationToAccessorMap) {
            const auto& accessor = model.accessors[accessorId]; 
            const auto& bufferView = model.bufferViews[accessor.bufferView];
            const auto& buffer = model.buffers[bufferView.buffer];
            const int attrSize = accessor.type != TINYGLTF_TYPE_SCALAR ? accessor.type : 1;
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
        const int indicesSize = indicesAccessor.type != TINYGLTF_TYPE_SCALAR ? indicesAccessor.type : 1;
        const int indicesByteStride = indicesAccessor.ByteStride(indicesBufferView);

        GLuint glBuffer;
        glGenBuffers(1, &glBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBufferView.byteLength,
                     &indicesBuffer.data.at(0) + indicesBufferView.byteOffset, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        // glDeleteBuffers(1, &glBuffer); Why the hell can't I delete these? :(

        auto result = std::make_unique<MeshNode>();
        result->SetMesh(std::make_unique<Common::Mesh>(VertexDataBase(VAO, indicesAccessor.count)));
        return result;
    }
}

std::unique_ptr<Node> processMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {
    // It's high time we figure out what type of mesh it is.
    // We need to figure it out based on custom properties.
    
    // I have no idea how mesh can have multiple Primitives so, ignore rest:
    const auto& material = model.materials[mesh.primitives[0].material];
    const auto& materialCustomProperties = material.extras;

    if (materialCustomProperties.Has(ProgramType) && materialCustomProperties.Get(ProgramType).IsString()) {
        const auto& materialType = materialCustomProperties.Get(ProgramType).Get<std::string>();
        if (materialType == ContourProgramType) {
            return processContourMesh(model, mesh);
        }
    }
    return nullptr;
}

std::unique_ptr<Node> processCamera(const tinygltf::Model& model, const tinygltf::Camera& camera) {
    return std::make_unique<CameraNode>(camera.perspective.yfov,  800.f/600.f);
}

std::unique_ptr<Node> processElement(const tinygltf::Model& model, const tinygltf::Node& node) {
    if (node.mesh != -1) {
        return processMesh(model, model.meshes[node.mesh]);
    } 

    if (node.camera != -1) {
        return processCamera(model, model.cameras[node.camera]);
    }

    return std::make_unique<Node>();
}

std::unique_ptr<Node> processModelNodes(const tinygltf::Model& model, const tinygltf::Node& node) {

    std::unique_ptr<Node> root = processElement(model, node);
    {
        const auto scaleVec = node.scale.size() == 0 ? glm::vec3(1.0) : glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
        const auto rotationQuat = node.rotation.size() == 0 ? glm::quat(1.0, 0.0, 0.0, 0.0) : glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
        const auto translationVec = node.translation.size() == 0 ? glm::vec3(0.0) : glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
        
        const auto scaleMatrix = glm::scale(glm::mat4(1.0), scaleVec);
        const auto rotateMatrix = glm::mat4_cast(rotationQuat);
        const auto translateMatrix = glm::translate(glm::mat4(1.0), translationVec);
        root->SetTransform(translateMatrix * rotateMatrix * scaleMatrix);
    }
    
    for (size_t i = 0; i < node.children.size(); ++i) {
        assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
        const auto& childNode = model.nodes[node.children[i]];
        auto processedNode = processModelNodes(model, childNode);
        root->AddChildNode(std::move(processedNode));
    }
    return root;
}

std::unique_ptr<Node> Importer::importGltf(const std::string& filename) {
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

    auto modelRoot = std::make_unique<Node>();
    const tinygltf::Scene &scene = model.scenes[model.defaultScene];

    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
        auto childNode = processModelNodes(model, model.nodes[scene.nodes[i]]);
        modelRoot->AddChildNode(std::move(childNode));
    }

    return modelRoot;
}

} // namespace Render
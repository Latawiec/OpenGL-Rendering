
#include "Importer.hpp"
#include <nlohmann/json.hpp>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_JSON
#include <tiny_gltf.h>
#include <glad/glad.h>

#include "Scene/Base/VertexData.hpp"
#include "Scene/Base/VertexAttribute.hpp"
#include "Scene/Base/Mesh.hpp"
#include "Scene/Base/Camera.hpp"
#include "Scene/Base/Texture.hpp"
#include "Scene/Base/DirectionalLight.hpp"
#include "Scene/Base/PointLight.hpp"
#include "Scene/NodeLink.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <unordered_map>
#include <iostream>
#include <string_view>
#include <array>

#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace Renderer {
namespace Importer {

namespace /*anonymous*/ {
    const static std::string LightsExtensionName = "KHR_lights_punctual";

    static const std::string PositionAttribute = "POSITION";
    static const std::string NormalsAttribute = "NORMAL";
    static const std::string TangentsAttribute = "TANGENT";
    static const std::string UvAttribute = "TEXCOORD_0";
    static const std::string EdgeColourAttribute = "COLOR_0";
    static const std::string JointsAttribute = "JOINTS_0";
    static const std::string JointsWeightsAttribute = "WEIGHTS_0";

}

Scene::Base::Mesh processMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // I assume single primitve.
    const auto primitive = mesh.primitives[0];

    const auto& positionAccessorId = primitive.attributes.at(PositionAttribute);
    const auto& normalAccessorId = primitive.attributes.at(NormalsAttribute);

    std::map<Scene::Base::VertexAttributeLocation, const int&> locationToAccessorMap {
        { Scene::Base::VertexAttributeLocation::POSITION, positionAccessorId },
        { Scene::Base::VertexAttributeLocation::NORMAL, normalAccessorId }
    };

    if (primitive.attributes.contains(TangentsAttribute)) {
        locationToAccessorMap.insert({ Scene::Base::VertexAttributeLocation::TANGENT, primitive.attributes.at(TangentsAttribute) });
    }

    if (primitive.attributes.contains(UvAttribute)) {
        locationToAccessorMap.insert({ Scene::Base::VertexAttributeLocation::UV_MAP, primitive.attributes.at(UvAttribute) });
    }

    if (primitive.attributes.contains(JointsAttribute) && primitive.attributes.contains(JointsWeightsAttribute)) {
        const auto& jointsAccessorId = primitive.attributes.at(JointsAttribute);
        const auto& jointsWeightsAccessorId = primitive.attributes.at(JointsWeightsAttribute);
        locationToAccessorMap.insert({ Scene::Base::VertexAttributeLocation::JOINT, jointsAccessorId });
        locationToAccessorMap.insert({ Scene::Base::VertexAttributeLocation::JOINT_WEIGHT, jointsWeightsAccessorId });
    }

    const auto& indicesAccessorId = primitive.indices;

    std::vector<GLuint> buffers = {};

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
        buffers.push_back(glBuffer);
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
    buffers.push_back(glBuffer);

    glBindVertexArray(0);

    return Scene::Base::Mesh{ Scene::Base::VertexDataBase(VAO, indicesAccessor.count, std::move(buffers)) };
}

Scene::Base::Camera processCamera(const tinygltf::Model& model, const tinygltf::Camera& camera) {
    return Scene::Base::Camera(0.1f, 1.2f, static_cast<float>(camera.perspective.yfov), 800.f/600.f, glm::mat4{1});
}

Scene::Base::Camera processCamera(const tinygltf::Model& model, const tinygltf::Camera& camera, const glm::mat4& cameraOrientation) {
    return Scene::Base::Camera(0.1f, 1.2f, static_cast<float>(camera.perspective.yfov), 800.f/600.f, cameraOrientation);
}

Scene::Base::Texture processTexture(const tinygltf::Model& model, const tinygltf::Image& image) {
    Scene::Base::Texture::Params parameters;
    parameters.width = image.width;
    parameters.height = image.height;
    if (image.component == 1) {
        parameters.internalFormat = Scene::Base::Texture::InternalFormat::R;
        parameters.format = Scene::Base::Texture::Format::R;
    } else 
    if (image.component == 2) {
        parameters.internalFormat = Scene::Base::Texture::InternalFormat::RG;
        parameters.format = Scene::Base::Texture::Format::RG;
    } else  
    if (image.component == 3) {
        parameters.internalFormat = Scene::Base::Texture::InternalFormat::RGB;
        parameters.format = Scene::Base::Texture::Format::RGB;
    } else 
    if (image.component == 4) {
        parameters.internalFormat = Scene::Base::Texture::InternalFormat::RGBA;
        parameters.format = Scene::Base::Texture::Format::RGBA;
    } else {
        throw 1;
    }

    parameters.type = Scene::Base::Texture::Type::UnsignedByte;
    
    return Scene::Base::Texture(image.image.data(), parameters);
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

void Importer::convertNodes(
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel)
{
    const auto nodesCount = gltfModel.nodes.size();
    nodeConversionData.convertedNodes.reserve(nodesCount);

    for (size_t i = 0; i < nodesCount; ++i) {
        const tinygltf::Node& gltfNode = gltfModel.nodes[i]; 
        glm::mat4 nodeTransform = processNodeTransform(gltfNode);
        // Blender exports additional node that contains Camera_Orientation, which is changing Y to be Up-axis.
        // We need to remember it and apply this transform before any other, to the View Transform.
        if (gltfNode.camera != -1) {
            nodeConversionData.cameraToOrientationNode[gltfNode.camera] = i;
            nodeTransform = glm::mat4{1};
        }

        // Also we need to check for lights orientation nodes...
        if (gltfNode.extensions.size() != 0 && gltfNode.extensions.contains(LightsExtensionName)) {
            const static std::string LightProperty_Name = "light";
            const auto lightId = gltfNode.extensions.at(LightsExtensionName).Get(LightProperty_Name).Get<int>();
            nodeConversionData.lightToOrientationNode[lightId] = i;
            nodeTransform = glm::mat4{1};
        }

        Scene::Base::Node convertedNode { nodeTransform };
        #ifndef NDEBUG
        convertedNode.SetName(gltfNode.name);
        #endif
        const SceneId sceneNodeId = scene.AddNode(std::move(convertedNode));
        nodeConversionData.convertedNodes.push_back(sceneNodeId);
    }
}

void Importer::convertMeshes(
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel)
{
    const auto meshesCount = gltfModel.meshes.size();
    meshConversionData.convertedMeshes.reserve(meshesCount);

    for (size_t i = 0; i < meshesCount; ++i) {
        const tinygltf::Mesh& gltfMesh = gltfModel.meshes[i];
        Scene::Base::Mesh convertedMesh = processMesh(gltfModel, gltfMesh); 
        const SceneId sceneMeshId = scene.AddMesh(std::move(convertedMesh));
        meshConversionData.convertedMeshes.push_back(sceneMeshId);
    }
}

void Importer::convertCameras (
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel)
{
    const auto camerasCount = gltfModel.cameras.size();
    cameraConversionData.convertedCameras.reserve(camerasCount);

    for (gltfId i = 0; i < camerasCount; ++i) {
        const tinygltf::Camera& gltfCamera = gltfModel.cameras[i];
        const bool cameraHasOrientationTransform = nodeConversionData.cameraToOrientationNode.contains(i);
        if (cameraHasOrientationTransform) {
            const tinygltf::Node& gltfNode = gltfModel.nodes[nodeConversionData.cameraToOrientationNode[i]];
            const glm::mat4 orientationTransform = processNodeTransform(gltfNode);
            Scene::Base::Camera convertedCamera = processCamera(gltfModel, gltfCamera, orientationTransform);
            cameraConversionData.convertedCameras.push_back(scene.AddCamera(std::move(convertedCamera)));
        } else {
            Scene::Base::Camera convertedCamera = processCamera(gltfModel, gltfCamera);
            cameraConversionData.convertedCameras.push_back(scene.AddCamera(std::move(convertedCamera)));
        }
    }
}

void Importer::convertTextures (
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel
    )
{
    const auto imagesCount = gltfModel.images.size();
    texturesConversionData.convertedImages.reserve(imagesCount);

    for (gltfId i = 0; i < imagesCount; ++i) {
        const tinygltf::Image& gltfImage = gltfModel.images[i];
        // Lets just assume it for safety now.
        assert(gltfImage.bits == 8);
        Scene::Base::Texture convertedTexture = processTexture(gltfModel, gltfImage);
        texturesConversionData.convertedImages.push_back(scene.AddTexture(std::move(convertedTexture)));
    }
}

void Importer::convertLights (
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel
    )
{
    // Lights are only available in gltf2 through an extension. So we need to fetch them from there.
    const static std::string LightType_Directional = "directional";
    const static std::string LightType_Point = "point";

    const int lightsCount = gltfModel.lights.size();
    for (int i=0; i < lightsCount; ++i) {
        const auto& light = gltfModel.lights[i];

        const glm::vec3 convertedColor { light.color[0], light.color[1], light.color[2] };
        const float convertedIntensity = light.intensity;
        const bool lightHasOrientationTransform = nodeConversionData.lightToOrientationNode.contains(i);
        const glm::mat4 orientationTransform = lightHasOrientationTransform ? processNodeTransform(gltfModel.nodes[nodeConversionData.lightToOrientationNode[i]]) : glm::mat4{1};

        if (light.type == LightType_Directional) {
                
            Scene::Base::DirectionalLight convertedDirectionalLight{ orientationTransform };
            convertedDirectionalLight.SetColor(convertedColor);
            convertedDirectionalLight.SetIntensity(convertedIntensity);
            lightsConversionData.convertedDirectionalLights.emplace(i, scene.AddDirectionalLight(std::move(convertedDirectionalLight)));
        }

        if (light.type == LightType_Point) {
                
            Scene::Base::PointLight convertedPointLight{ orientationTransform };
            convertedPointLight.SetColor(convertedColor);
            convertedPointLight.SetIntensity(convertedIntensity);
            lightsConversionData.convertedPointLights.emplace(i, scene.AddPointLight(std::move(convertedPointLight)));

        }
    }
}

void Importer::convertMaterials (
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel
    )
{
    const static std::string Property_Dithering = "Dithering";
    const static std::string Property_CastShadow = "CastShadow";

    const static std::string Dithering_Bayer4x4 = "Bayer4x4";
    const static std::string Dithering_Bayer8x8 = "Bayer8x8";
    const static std::string Dithering_BlueNoise = "BlueNoise";
    const static std::string Dithering_WhiteNoise = "WhiteNoise";

    const auto materialsCount = gltfModel.materials.size();
    materialsConversionData.convertedMaterials.reserve(materialsCount);

    for (gltfId i = 0; i < materialsCount; ++i) {
        const tinygltf::Material& gltfMaterial = gltfModel.materials[i];
        const gltfId albedoTexture = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
        const gltfId metallicRoughnessTexture = gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index;
        const gltfId normalTexture = gltfMaterial.normalTexture.index;
        
        Scene::Base::Material material;
        if (albedoTexture != -1) {
            const gltfId albedoTextureImage = gltfModel.textures[albedoTexture].source;
            const Scene::Base::Texture::IdType albedoId = texturesConversionData.convertedImages[albedoTextureImage];
            material.setTexture<Scene::Base::Material::ETexture::Albedo>(albedoId);
        }

        if (metallicRoughnessTexture != -1) {
            const gltfId metallicRoughnessImage = gltfModel.textures[metallicRoughnessTexture].source;
            const Scene::Base::Texture::IdType metallicRoughnessId = texturesConversionData.convertedImages[metallicRoughnessImage];
            material.setTexture<Scene::Base::Material::ETexture::MetallicRoughness>(metallicRoughnessId);
        }

        if (normalTexture != -1) {
            const gltfId normalImage = gltfModel.textures[normalTexture].source;
            const Scene::Base::Texture::IdType normalId = texturesConversionData.convertedImages[normalImage];
            material.setTexture<Scene::Base::Material::ETexture::Normal>(normalId);
        }

        if (gltfMaterial.extras.Has(Property_Dithering)) {
            const auto& ditheringType = gltfMaterial.extras.Get(Property_Dithering).Get<std::string>();
            if (ditheringType == Dithering_Bayer4x4) {
                material.setDithering(Scene::Base::Material::EDithering::Bayer4x4);
            }
            else
            if (ditheringType == Dithering_Bayer8x8) {
                material.setDithering(Scene::Base::Material::EDithering::Bayer8x8);
            }
            else
            if (ditheringType == Dithering_BlueNoise) {
                material.setDithering(Scene::Base::Material::EDithering::BlueNoise);
            }
            else 
            if (ditheringType == Dithering_WhiteNoise) {
                material.setDithering(Scene::Base::Material::EDithering::WhiteNoise);
            }
        }

        if (gltfMaterial.extras.Has(Property_CastShadow)) {
            const auto& castShadow = gltfMaterial.extras.Get(Property_CastShadow).Get<bool>();
            material.setCastingShadow(castShadow);
        } else {
            material.setCastingShadow(true);
        }

        materialsConversionData.convertedMaterials.push_back(std::move(scene.AddMaterial(std::move(material))));
    }
}

Scene::Base::Skin::BoneLink Importer::traverseSkinNodes(
    std::unordered_map<gltfId, size_t>& skinBoneNodesMissing,
    const tinygltf::Model& gltfModel,
    const gltfId nodeIndex)
{
    Scene::Base::Skin::BoneLink result { skinBoneNodesMissing[nodeIndex] };
    skinBoneNodesMissing.erase(nodeIndex);

    // Check if we done.
    if (skinBoneNodesMissing.empty()) return result;

    const auto node = gltfModel.nodes[nodeIndex];
    const auto childNodesCount = node.children.size();
    for (size_t i = 0; i < childNodesCount; ++i) {
        const auto gltfChildNodeId = node.children[i];
        if (skinBoneNodesMissing.contains(gltfChildNodeId)) {
            result.childBoneLinks.push_back(traverseSkinNodes(skinBoneNodesMissing, gltfModel, gltfChildNodeId));
            // Check if we done.
            if (skinBoneNodesMissing.empty()) return result;
        }
    }

    return result;
}

void Importer::convertSkins (
    Scene::Scene& scene,
    const tinygltf::Model& gltfModel)
{
    const auto skinsCount = gltfModel.skins.size();
    skinConversionData.convertedSkins.reserve(skinsCount);

    for (size_t i = 0; i < skinsCount; ++i) {
        const auto& invBindMatricesAccessorId = gltfModel.skins[i].inverseBindMatrices;
        const auto& invBindMatricesAccessor = gltfModel.accessors[invBindMatricesAccessorId];
        const auto& invBindMatricesBufferView = gltfModel.bufferViews[invBindMatricesAccessor.bufferView];
        const auto& invBindMatricesBuffer = gltfModel.buffers[invBindMatricesBufferView.buffer];
        const glm::mat4* invBindMatricesArray = reinterpret_cast<const glm::mat4*>(&invBindMatricesBuffer.data.at(0) + invBindMatricesBufferView.byteOffset);

        const tinygltf::Skin& gltfSkin = gltfModel.skins[i];

        std::unordered_map<Scene::Base::Skin::JointTransformIndex, Scene::Base::Skin::Bone> skinBones;
        skinBones.reserve(gltfSkin.joints.size());

        std::unordered_map<gltfId, size_t> skinBoneNodesMissing; // Mapped to indices.
        skinBoneNodesMissing.reserve(gltfSkin.joints.size());

        for (size_t j = 0; j < gltfSkin.joints.size(); ++j) {
            const auto glftJointNodeId = gltfSkin.joints[j];
            skinBoneNodesMissing[gltfSkin.joints[j]] = j;
            skinBones[j] = { invBindMatricesArray[j], nodeConversionData.convertedNodes[glftJointNodeId] };
        }
        
        // I assume that first node in this list is root node (please...);
        const auto skinRootNodeId = gltfSkin.joints[0];
        Scene::Base::Skin::BoneLink skinRootLink = traverseSkinNodes(skinBoneNodesMissing, gltfModel, skinRootNodeId);

        Scene::Base::Skin convertedSkin { std::move(skinBones), std::move(skinRootLink) };
        skinConversionData.convertedSkins.push_back(scene.AddSkin(std::move(convertedSkin)));
    }
}

Scene::NodeLink Importer::traverseNodes(Scene::Scene& scene, const tinygltf::Model& gltfModel, const size_t glftNodeId) {
    const auto& nodeId = nodeConversionData.convertedNodes[glftNodeId];
    Scene::NodeLink nodeLink { nodeId };
    const auto& gltfNode = gltfModel.nodes[glftNodeId];

    // Scene elements
    if (gltfNode.mesh != -1) {
        const auto& gltfMesh = gltfModel.meshes[gltfNode.mesh];
        Scene::SceneObject obj;
        obj.nodeId = nodeId;
        obj.meshId = meshConversionData.convertedMeshes[gltfNode.mesh];;

        if (gltfNode.skin != -1) {
            obj.skinId = skinConversionData.convertedSkins[gltfNode.skin];
        }

        if (gltfMesh.primitives[0].material != -1) {
            obj.materialId = materialsConversionData.convertedMaterials[gltfMesh.primitives[0].material];
        }

        scene.AddSceneObject(obj);
    }

    // Camera
    if (gltfNode.camera != -1) {
        const auto& cameraId = cameraConversionData.convertedCameras[gltfNode.camera];
        scene.AddSceneView({ nodeId, cameraId });
    }

    // Light
    if (gltfNode.extensions.contains(LightsExtensionName)){
        const static std::string LightPropertyName = "light";
        const auto& lightObject = gltfNode.extensions.at(LightsExtensionName).Get(LightPropertyName);
        const gltfId lightId = lightObject.Get<int>();

        // Distinguish light type by the container holding it.
        if (lightsConversionData.convertedDirectionalLights.contains(lightId)) {
            Scene::SceneLight light;
            light.nodeId = nodeId;
            light.directionalLightId = lightsConversionData.convertedDirectionalLights.at(lightId);
            scene.AddSceneLight(light);
        } 
    }

    for (const auto& childNodeId : gltfNode.children) {
        nodeLink.AddChild(traverseNodes(scene, gltfModel, childNodeId));
    }

    return nodeLink;
}

void Importer::clearCache() {
    nodeConversionData = {};
    meshConversionData = {};
    cameraConversionData = {};
    skinConversionData = {};
    texturesConversionData = {};
    materialsConversionData = {};
}

bool Importer::importGltf(const std::string& filename, Scene::Scene& scene) {
    tinygltf::Model gltfModel;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, filename);
    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
    }

    if (!ret) {
        return ret;
    }

    convertNodes(scene, gltfModel);
    convertMeshes(scene, gltfModel);
    convertCameras(scene, gltfModel);
    convertSkins(scene, gltfModel);
    convertTextures(scene, gltfModel);
    convertMaterials(scene, gltfModel);
    convertLights(scene, gltfModel);
    // Now that all is converted, set scene hierarchy.
    const tinygltf::Scene &gltfScene = gltfModel.scenes[gltfModel.defaultScene];

    for (size_t i = 0; i < gltfScene.nodes.size(); ++i) {
        assert((gltfScene.nodes[i] >= 0) && (gltfScene.nodes[i] < gltfModel.nodes.size()));
        const auto nodeId = gltfScene.nodes[i];
        scene.AddNodeHierarchy(traverseNodes(scene, gltfModel, nodeId));
    }

    clearCache();

    return true;
}

} // namespace Importer
} // namespace Renderer
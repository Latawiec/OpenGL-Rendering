#pragma once

#ifndef NDEBUG
#include <iostream>
#endif

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Node.hpp"
#include "Skin.hpp"
#include "Texture.hpp"
#include "NodeLink.hpp"
#include "IdGenerator.hpp"

#include <array>
#include <unordered_map>

namespace Render {
namespace Common {

struct Scene {
    using NodeIdType = IdGenerator::Type;
    using MeshIdType = IdGenerator::Type;
    using TextureIdType = IdGenerator::Type;
    using MaterialIdType = IdGenerator::Type;
    using CameraIdType = IdGenerator::Type;
    using SkinIdType = IdGenerator::Type;
    using LightIdType = IdGenerator::Type;

    // Helper
    struct SkinnedMesh {
        MeshIdType meshId;
        SkinIdType skinId;
    };

    // I'll have to move it later on
    struct BasicMaterial { 
        enum class EDithering : uint8_t {
            NONE = 0,
            Bayer4x4,
            Bayer8x8,
            Bayer16x16,
            Bayer32x32,
            Bayer64x64,
            WhiteNoise,
            BlueNoise
        };

        enum class ETexture : uint8_t {
            EdgeMap = 0,
            Albedo,
            Emissive,
            Normal,
            MetallicRoughness, // Metallic -> Blue; Rougness -> Green
            Occlusion,
            SIZE
        };

        template<ETexture TextureType>
        bool hasTexture() const {
            return _textures[size_t(TextureType)].has_value();
        }

        template<ETexture TextureType>
        const TextureIdType getTexture() const {
            return _textures[size_t(TextureType)];
        }

        template<ETexture TextureType>
        void setTexture(TextureIdType texture) {
            _textures[size_t(TextureType)] = texture;
        }

        EDithering getDithering() const { return _dithering; }
        void setDithering(EDithering value) { _dithering = value; }

        private:
        std::array<TextureIdType, size_t(ETexture::SIZE)> _textures = {};
        EDithering _dithering = EDithering::NONE;
    };

    Scene();

    NodeIdType AddNode(Node&& node);
    Node& GetNode(const NodeIdType& id);
    const Node& GetNode(const NodeIdType& id) const;

    MeshIdType AddMesh(Mesh&& mesh);
    Mesh& GetMesh(const MeshIdType& id);
    const Mesh& GetMesh(const MeshIdType& id) const;

    TextureIdType AddTexture(Texture&& texture);
    Texture& GetTexture(const TextureIdType& id);
    const Texture& GetTexture(const TextureIdType& id) const;

    MaterialIdType AddMaterial(BasicMaterial&& material);
    BasicMaterial& GetMaterial(const MaterialIdType& id);
    const BasicMaterial& GetMaterial(const MaterialIdType& id) const;

    CameraIdType AddCamera(Camera&& camera);
    Camera& GetCamera(const CameraIdType& id);
    const Camera& GetCamera(const CameraIdType& id) const;

    SkinIdType AddSkin(Skin&& skin);
    Skin& GetSkin(const SkinIdType& id);
    const Skin& GetSkin(const SkinIdType& id) const;
    const std::unordered_map<SkinIdType, Skin>& GetSkins() const;

    // void AddDirectionalLight(Directional::Light light);
    // void AddShadow(const LightIdType& id, uint8_t quality);
    // void RemoveShadow(const LightIdType& id);
    // Directional::Light& GetDirectionalLight(const LightIdType& id);
    // const Directional::Light& GetDirectionalLight(const LightIdType& id) const;
    // const DepthBuffer& GetDirectionalShadow(const LightIdType& id) const;

    void AddNodeHierarchy(NodeLink&& nodeLink);
    NodeLink& GetNodeHierarchy();
    const NodeLink& GetNodeHierarchy() const;

    void AttachCamera(const NodeIdType& nodeId, const CameraIdType& cameraId);
    void DetachCamera(const NodeIdType& nodeId);
    const std::unordered_map<NodeIdType, CameraIdType>& GetCameraNodes() const;

    void AttachStaticMesh(const NodeIdType& nodeId, const MeshIdType& meshId);
    void DetachStaticMesh(const NodeIdType& nodeId);
    const std::unordered_map<NodeIdType, MeshIdType>& GetStaticMeshNodes() const;

    void AttachSkinnedMesh(const NodeIdType& nodeId, const SkinnedMesh skinnedMesh);
    void DetachSkinnedMesh(const NodeIdType& nodeId);
    const std::unordered_map<NodeIdType, SkinnedMesh>& GetSkinnedMeshNodes() const;

    void AttachLight(const NodeIdType& nodeId, const LightIdType& lightId);
    void DetachLight(const NodeIdType& nodeId, const LightIdType& lightId);
    // const std::unordered_map<NodeIdType, Directional::Light>& GetDirectionalLightNodes() const;
    // const std::unordered_map<LightIdType, DepthBuffer>& GetDirectionalShadowMaps() const;

private:
    IdGenerator _nodeIdGenerator;
    IdGenerator _meshIdGenerator;
    IdGenerator _textureIdGenerator;
    IdGenerator _materialIdGenerator;
    IdGenerator _cameraIdGenerator;
    IdGenerator _skinIdGenerator;
    IdGenerator _lightIdGenerator;

    // Raw data
    std::unordered_map<NodeIdType, Node> _nodes;
    std::unordered_map<MeshIdType, Mesh> _meshes;
    std::unordered_map<CameraIdType, Camera> _cameras;
    std::unordered_map<SkinIdType, Skin> _skins;
    std::unordered_map<TextureIdType, Texture> _textures;
    std::unordered_map<MaterialIdType, BasicMaterial> _materials;

    // std::unordered_map<LightIdType, Directional::Light> _directionalLights;
    // std::unordered_map<LightIdType, DepthBuffer> _directionalShadowMaps;

    // Data connections
    std::unordered_map<NodeIdType, CameraIdType> _cameraNodes;
    std::unordered_map<NodeIdType, MeshIdType> _staticMeshNodes;
    std::unordered_map<NodeIdType, SkinnedMesh> _skinnedMeshNodes;
    std::unordered_map<NodeIdType, LightIdType> _directionalLightNodes;


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

} // namespace Common
} // namespace Render
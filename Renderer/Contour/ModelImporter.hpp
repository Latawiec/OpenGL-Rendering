#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#ifndef NDEBUG
#include <iostream>
#endif

namespace Render {
namespace Contour {

class ModelImporter {

public:
    static void import(const std::string& data) {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(data, aiProcess_Triangulate);
    
        #ifndef NDEBUG
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Failed to import file: " << data << std::endl;
        }
        #endif   
    }
};

} // namespace Contour
} // namespace Render
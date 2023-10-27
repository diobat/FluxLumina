#pragma once

// STD library includes
#include <map>
#include <string>
#include <vector>
#include <set>

// First-party includes
#include "util\Arithmetic.h"
#include "rendering\Mesh.h"
#include "rendering\Texture.h"

class MeshLibrary
{
public:

    // Meshes
    void addMesh(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& mesh);
    std::vector<std::shared_ptr<Mesh>> getMeshes(const std::string& name);
    std::shared_ptr<Mesh> getMesh(boost::uuids::uuid id) const;
    bool isMeshLoaded(const std::string& name); 

    // Textures
    void addTexture(const Texture& texture);
    const std::vector<Texture>& getLoadedTextures();
private:
    std::map<std::size_t, std::vector<std::shared_ptr<Mesh>>> _meshes;
    std::vector<Texture> _loadedTextures;
};
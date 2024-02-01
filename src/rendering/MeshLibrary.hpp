#pragma once

// STD library includes
#include <map>
#include <string>
#include <vector>
#include <set>

// First-party includes
#include "util/Arithmetic.hpp"
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"

class MeshLibrary
{
public:

    // Meshes
    void addMesh(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& mesh);
    void addMesh(const std::size_t& hash, const std::shared_ptr<Mesh>& mesh);

    void updateMesh(boost::uuids::uuid meshID, std::shared_ptr<Mesh> newMesh);

    void initializeMesh(std::shared_ptr<Mesh> mesh);

    std::vector<std::shared_ptr<Mesh>> getMeshes(const std::string& name);
    std::vector<std::shared_ptr<Mesh>> getMeshes(const std::size_t& hash);
    std::shared_ptr<Mesh> getMesh(boost::uuids::uuid id) const;
    bool isMeshLoaded(const std::string& name); 
    bool isMeshLoaded(const std::size_t& hash);

    // Textures
    void addTexture(const Texture& texture);
    const std::vector<Texture>& getLoadedTextures();
private:
    std::map<std::size_t, std::vector<std::shared_ptr<Mesh>>> _meshes;
    std::vector<Texture> _loadedTextures;
    
};
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

    void addMesh(const std::string& name, const std::vector<Mesh>& mesh);
    const std::vector<Mesh>& getMeshes(const std::string& name);

    std::vector<Texture>& getLoadedTextures();

    bool isMeshLoaded(const std::string& name); 

private:
    std::map<std::size_t, std::vector<Mesh>> _meshes;
    std::vector<Texture> _loadedTextures;
};
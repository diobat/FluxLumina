#include "rendering\MeshLibrary.hpp"


void MeshLibrary::addMesh(const std::string& name, const std::vector<Mesh>& mesh)
{
    _meshes[Math::calculateHash(name)].insert(_meshes[Math::calculateHash(name)].end(), mesh.begin(), mesh.end());
}

const std::vector<Mesh>& MeshLibrary::getMeshes(const std::string& name)
{
    return _meshes[Math::calculateHash(name)];
}

std::vector<Texture>& MeshLibrary::getLoadedTextures()
{
    return _loadedTextures;
}

bool MeshLibrary::isMeshLoaded(const std::string& name)
{
    return _meshes.find(Math::calculateHash(name)) != _meshes.end();
}
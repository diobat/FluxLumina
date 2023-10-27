#include "rendering\MeshLibrary.hpp"


void MeshLibrary::addMesh(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& mesh)
{
    _meshes[Math::calculateHash(name)].insert(_meshes[Math::calculateHash(name)].end(), mesh.begin(), mesh.end());
}

std::vector<std::shared_ptr<Mesh>> MeshLibrary::getMeshes(const std::string& name)
{
    return _meshes[Math::calculateHash(name)];
}

std::shared_ptr<Mesh> MeshLibrary::getMesh(boost::uuids::uuid id) const
{
    for (auto& mesh : _meshes)
    {
        for (auto& m : mesh.second)
        {
            if (m->_id == id)
            {
                return m;
            }
        }
    }
    throw std::runtime_error("MeshLibrary::getMesh() - Mesh not found");
}

std::vector<Texture>& MeshLibrary::getLoadedTextures()
{
    return _loadedTextures;
}

bool MeshLibrary::isMeshLoaded(const std::string& name)
{
    return _meshes.find(Math::calculateHash(name)) != _meshes.end();
}
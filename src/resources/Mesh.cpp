#include "resources/Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, bool hasTransparency)    :
    _vertices(vertices),
    _indices(indices),
    _textures(textures),
    _hasTransparency(hasTransparency)
{
    _id = boost::uuids::random_generator()();
}

void Mesh::attachTexture(std::vector<Texture> textures)
{
    _textures.insert(_textures.end(), textures.begin(), textures.end());
}

const std::vector<Vertex>& Mesh::vertices() const
{
    return _vertices;
}
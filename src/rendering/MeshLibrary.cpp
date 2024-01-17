#include "rendering/MeshLibrary.hpp"

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"


void MeshLibrary::addMesh(const std::string& name, const std::vector<std::shared_ptr<Mesh>>& meshes)
{

    for(auto& mesh : meshes)
    {
        initializeMesh(mesh);
    }

    _meshes[Math::calculateHash(name)] = meshes;
}

void MeshLibrary::addMesh(const std::size_t& hash, const std::shared_ptr<Mesh>& mesh)
{
    initializeMesh(mesh);

    if(_meshes.find(hash) == _meshes.end())
    {
        _meshes[hash] = std::vector<std::shared_ptr<Mesh>>({mesh});
    }
}

void MeshLibrary::initializeMesh(const std::shared_ptr<Mesh> mesh)
{
    // create buffers/arrays
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->_vertices.size() * sizeof(Vertex), &mesh->_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->_indices.size() * sizeof(unsigned int), &mesh->_indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // vertex color info
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));
    // vertex tangent info
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));

    glBindVertexArray(0);

}

std::vector<std::shared_ptr<Mesh>> MeshLibrary::getMeshes(const std::string& name)
{
    return _meshes[Math::calculateHash(name)];
}

std::vector<std::shared_ptr<Mesh>> MeshLibrary::getMeshes(const std::size_t& hash)
{
    return _meshes[hash];
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

const std::vector<Texture>& MeshLibrary::getLoadedTextures()
{
    return _loadedTextures;
}

bool MeshLibrary::isMeshLoaded(const std::string& name)
{
    return _meshes.find(Math::calculateHash(name)) != _meshes.end();

}

bool MeshLibrary::isMeshLoaded(const std::size_t& hash)
{
    return _meshes.find(hash) != _meshes.end();
}

void MeshLibrary::addTexture(const Texture& texture)
{
    _loadedTextures.push_back(texture);
}
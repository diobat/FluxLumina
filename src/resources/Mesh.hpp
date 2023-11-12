#pragma once

// STL headers
#include <vector>

// First-party headers
#include <resources/Texture.hpp>

// Third-party headers
#include <glm/glm.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // Color
    glm::vec3 Color;
    // Tangent
    glm::vec3 Tangent;
};

class Mesh
{
public:
    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, bool hasTransparency = false)    :
        _vertices(vertices),
        _indices(indices),
        _textures(textures),
        _hasTransparency(hasTransparency)
    {
        _id = boost::uuids::random_generator()();
    }

    void attachTexture(std::vector<Texture> textures)
    {
        _textures.insert(_textures.end(), textures.begin(), textures.end());
    }

    // Structural Data
    boost::uuids::uuid _id;

    // Mesh Data
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;
    bool _hasTransparency = false;

    // Rendering Data
    unsigned int VAO, VBO, EBO;
};

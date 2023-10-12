#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <vector>
#include <rendering/texture.h>

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
};

class Mesh
{
public:
    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->_vertices = vertices;
        this->_indices = indices;
        this->_textures = textures;
    }

    void attachTexture(std::vector<Texture> textures)
    {
        _textures.insert(_textures.end(), textures.begin(), textures.end());
    }

    /*  Mesh Data  */
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;
    unsigned int VAO, VBO, EBO;

};

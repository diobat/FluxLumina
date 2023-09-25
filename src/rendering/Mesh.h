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
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO, VBO, EBO;

    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
    }

};

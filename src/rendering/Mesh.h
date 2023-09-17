/**
* Code taken from www.learnopengl.com
* Slightly modified by Tomasz Gałaj 2018
**/

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <vector>

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
    unsigned int VAO, VBO, EBO;

    /*  Functions  */
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }

};
#endif

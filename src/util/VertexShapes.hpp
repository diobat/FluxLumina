#pragma once

#include "rendering/GLFW_Wrapper.hpp"
#include <glm/glm.hpp>

#include <vector>

namespace
{
    // Quad
    bool quadVertices_init = false;
    unsigned int quadVAO_nr, quadVBO_nr;
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // Cube
    bool cubeVertices_init = false;
    unsigned int cubeVAO_nr, cubeVBO_nr;
    float cubeVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
        };


    // Sphere
    bool sphereVertices_init = false;
    unsigned int sphereVAO_nr, sphereVBO_nr, sphereEBO_nr;
    unsigned int sphereVertexCount = 0;
    unsigned int sphereIndexCount = 0;
    // A function that returns the openGL VAO for a sphere object, it takes the radius and the number of stacks and slices as parameters.
    // The sphere is centered at the origin and drawn using triangle strips.
    // The sphere is drawn using the following parametric equation:
    // x = r * cos(theta) * sin(phi)
    // y = r * sin(theta) * sin(phi)
    // z = r * cos(phi)
    // where theta is the angle around the y axis and phi is the angle around the x axis.
    unsigned int sphereVAO(unsigned int stacks, unsigned int slices, float radius)
    {
        // Calculate the number of vertices in the sphere
        sphereVertexCount = (stacks + 1) * (slices + 1);
        // Calculate the number of indices in the sphere
        sphereIndexCount = stacks * slices * 6;

        // Create a vector to hold the vertices
        std::vector<float> vertices(sphereVertexCount * 3);
        // Create a vector to hold the indices
        std::vector<unsigned int> indices(sphereIndexCount);

        // Calculate the vertex positions and the texture coordinates
        for (unsigned int i = 0; i <= stacks; i++)
        {
            for (unsigned int j = 0; j <= slices; j++)
            {
                float theta = (float)i / (float)stacks * 2.0f * glm::pi<float>();
                float phi = (float)j / (float)slices * glm::pi<float>();

                float x = radius * glm::cos(theta) * glm::sin(phi);
                float y = radius * glm::sin(theta) * glm::sin(phi);
                float z = radius * glm::cos(phi);

                vertices[(i * (slices + 1) + j) * 3] = x;
                vertices[(i * (slices + 1) + j) * 3 + 1] = y;
                vertices[(i * (slices + 1) + j) * 3 + 2] = z;
            }
        }

        // Calculate the indices
        for (unsigned int i = 0; i < stacks; i++)
        {
            for (unsigned int j = 0; j < slices; j++)
            {
                indices[(i * slices + j) * 6] = i * (slices + 1) + j;
                indices[(i * slices + j) * 6 + 1] = i * (slices + 1) + j + 1;
                indices[(i * slices + j) * 6 + 2] = (i + 1) * (slices + 1) + j;
                indices[(i * slices + j) * 6 + 3] = i * (slices + 1) + j + 1;
                indices[(i * slices + j) * 6 + 4] = (i + 1) * (slices + 1) + j + 1;
                indices[(i * slices + j) * 6 + 5] = (i + 1) * (slices + 1) + j;
            }
        }

        // Generate and bind the VAO
        glGenVertexArrays(1, &sphereVAO_nr);
        glBindVertexArray(sphereVAO_nr);

        // Generate and bind the VBO
        glGenBuffers(1, &sphereVBO_nr);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO_nr);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Generate and bind the EBO
        glGenBuffers(1, &sphereEBO_nr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO_nr);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(vertices.size() * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind the VAO
        glBindVertexArray(0);

        return sphereVAO_nr;
    } 

}

namespace shapes
{

    namespace quad
    {
        unsigned int VAO()
        {
            if (!quadVertices_init)
            {
                glGenVertexArrays(1, &quadVAO_nr);
                glGenBuffers(1, &quadVBO_nr);
                glBindVertexArray(quadVAO_nr);
                glBindBuffer(GL_ARRAY_BUFFER, quadVBO_nr);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
                quadVertices_init = true;

            }
            return quadVAO_nr;
        }
    }

    namespace cube
    {

        unsigned int VAO()
        {
            if (!cubeVertices_init)
            {
                glGenVertexArrays(1, &cubeVAO_nr);
                glGenBuffers(1, &quadVBO_nr);
                glBindVertexArray(cubeVAO_nr);
                glBindBuffer(GL_ARRAY_BUFFER, quadVBO_nr);
                glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                cubeVertices_init = true;

            }
            return cubeVAO_nr;
        }

    }

    namespace sphere
    {
        unsigned int VAO()
        {
            if(!sphereVertices_init)
            {
                sphereVAO(16, 8, 1.0f);
                sphereVertices_init = true;
            }
            return sphereVAO_nr;
        }

        unsigned int vertexCount()
        {
            return sphereVertexCount;
        }

        unsigned int indexCount()
        {
            return sphereIndexCount;
        }
    }
}
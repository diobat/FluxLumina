#pragma once

// STL headers
#include <array>

// First-party headers
#include "resources/Texture.hpp"

enum E_CUBEMAP_FACES : unsigned int
{
    E_CUBEMAP_FACE_POSITIVE_X = 0,
    E_CUBEMAP_FACE_NEGATIVE_X = 1,
    E_CUBEMAP_FACE_POSITIVE_Y = 2,
    E_CUBEMAP_FACE_NEGATIVE_Y = 3,
    E_CUBEMAP_FACE_POSITIVE_Z = 4,
    E_CUBEMAP_FACE_NEGATIVE_Z = 5
};

class Cubemap
{
public:
    Cubemap();

    unsigned int VAO, VBO;
    float _vertices[108];
    void addTexture(std::array<Texture, 6> &textures);
    Texture& getTexture();

private:
    Texture _texture;
};
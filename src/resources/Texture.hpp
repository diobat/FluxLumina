#pragma once

// STL headers
#include <string>

// Third-party headers
#include <glad/glad.h>

enum E_TexureType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
    CUBEMAP
};

class Texture
{
public:
    Texture();
    
    GLuint _id;
    E_TexureType _type;
    std::string _path;
    int _width, _height, _components;

    GLenum _colorChannels;
    bool _useLinear;
    bool _isLoaded = false;

    unsigned char * _pixels = nullptr;
};

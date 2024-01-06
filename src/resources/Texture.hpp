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
    LIGHTMAP,
    ROUGHNESS,
    CUBEMAP
};

class TextureBase
{
public:
    TextureBase();    
    GLuint _id;
    E_TexureType _type;
    std::string _path;
    int _width, _height, _components;

    GLenum _colorChannels;
    bool _useLinear;
    bool _isLoaded = false;
};

class Texture : public TextureBase
{
public:
    unsigned char * _pixels = nullptr;
};

class TextureHDR : public TextureBase
{ 
public:  
    float * _pixels = nullptr;
};

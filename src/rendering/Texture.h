/** 
 * Copyright (C) 2018 Tomasz Gałaj
 **/

#pragma once
#include <string>
#include <glad/glad.h>

enum E_TexureType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

class Texture
{
public:
    Texture();
    // ~Texture();
    
    GLuint _id;
    E_TexureType _type;
    std::string _path;
    int _width, _height, _components;

    GLenum _colorChannels;
    bool _useLinear;
    bool _isLoaded = false;

    unsigned char * _pixels = nullptr;
};

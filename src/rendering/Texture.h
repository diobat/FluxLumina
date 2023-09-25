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
    ~Texture();
    
    void bind(int index = 0) const
    {
        if(_id != 0)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, _id);
        }
    }

    
    GLenum _colorChannels;
    bool use_linear;
    int _width, _height, _components;
    bool is_loaded = false;
    GLuint _id;
    unsigned char * _pixels = nullptr;
};

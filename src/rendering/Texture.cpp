/** 
 * Copyright (C) 2018 Tomasz Gałaj
 **/

#include <stb_image.h>

#include "Texture.h"
#include <iostream>
#include <helpers/RootDir.h>

Texture::Texture()
    : _useLinear(true), _id(0)
{
}

// Texture::~Texture()
// {
//     if(_id != 0)
//     {
//         glDeleteTextures(1, &_id);
//         _id = 0;
//     }
// }

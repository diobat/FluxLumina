#include "resources/Texture.hpp"


TextureBase::TextureBase()
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

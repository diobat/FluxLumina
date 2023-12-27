#include "rendering/libraries/TextureLibrary.hpp"

#include "rendering/GraphicalEngine.hpp"
#include "rendering/shader/ShaderLibrary.hpp"
#include "rendering/Settings.hpp"

#include "util/VertexShapes.hpp"

#include <stb_image.h>

TextureLibrary::TextureLibrary(GraphicalEngine* engine)
{
    _ranFrom = engine;
}

std::shared_ptr<Texture> TextureLibrary::generate_GL_texture(Texture &texture)
{

    // load and create a texture
    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_2D, texture._id);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, texture._colorChannels, texture._width, texture._height, 0, texture._colorChannels, GL_UNSIGNED_BYTE, texture._pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // We can free the data now as it is stored in the GPU
    stbi_image_free(texture._pixels);

    std::shared_ptr<Texture> texturePtr = std::make_shared<Texture>(texture);

    // Add it to the container
    _textures[texture._id] = texturePtr;

    return texturePtr;
}

std::shared_ptr<TextureHDR> TextureLibrary::generate_GL_textureHDR(TextureHDR& texture)
{

    // load and create a texture
    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_2D, texture._id);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    // load image, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texture._width, texture._height, 0, texture._colorChannels, GL_FLOAT, texture._pixels);

    // We can free the data now as it is stored in the GPU
    stbi_image_free(texture._pixels);

    std::shared_ptr<TextureHDR> texturePtr = std::make_shared<TextureHDR>(texture);

    // Add it to the container
    _texturesHDR[texture._id] = texturePtr;

    return texturePtr;
}

std::shared_ptr<Cubemap> TextureLibrary::generate_GL_cubemap(Cubemap &cubemap, const std::array<Texture, 6>& textures)
{

    cubemap.VAO = shapes::cube::VAO();

    // Setup textures of the cubemap

    Texture& cubemapTexture = cubemap.getTexture();
    glGenTextures(1, &cubemapTexture._id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture._id);

    // load and create a texture
    for (int i(0); i < textures.size() ; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textures[i]._colorChannels, textures[i]._width, textures[i]._height, 0, textures[i]._colorChannels, GL_UNSIGNED_BYTE, textures[i]._pixels);

        stbi_image_free(textures[i]._pixels);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // Unbind texture

    std::shared_ptr<Cubemap> texturePtr = std::make_shared<Cubemap>(cubemap);

    return texturePtr;

}

void TextureLibrary::bindTextures(std::shared_ptr<Mesh> mesh)
{
    std::shared_ptr<ShaderLibrary> shaderLibrary = _ranFrom->getShaderLibrary();
    std::shared_ptr<Settings> settings = _ranFrom->getSettings();

    shaderLibrary->setUniformInt("sampleFromDiffuse", 0);
    shaderLibrary->setUniformInt("sampleFromSpecular", 0);
    shaderLibrary->setUniformInt("sampleFromNormal", 0);
    shaderLibrary->setUniformInt("sampleFromHeight", 0);

    shaderLibrary->setUniformFloat("material.shininess", 4.5f);

    if (mesh->_textures.size() == 0)
    {
        return;
    }

    for (int i = 0; i < mesh->_textures.size(); i++)
    {

        switch (mesh->_textures[i]._type)
        {
        case DIFFUSE:
            shaderLibrary->setUniformInt("sampleFromDiffuse", 1);
            shaderLibrary->setUniformInt("material.diffuse", 1);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case SPECULAR:
            shaderLibrary->setUniformInt("sampleFromSpecular", 1);
            shaderLibrary->setUniformInt("material.specular", 2);
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case NORMAL:
            if(settings->getNormalMapping() == E_Setting::ON)
            {
                shaderLibrary->setUniformInt("sampleFromNormal", 1);
                shaderLibrary->setUniformInt("material.normal", 3);
                glActiveTexture(GL_TEXTURE0 + 3);
                glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            }
            break;
        case HEIGHT:
            if(settings->getHeightMapping() == E_Setting::ON)
            {
                shaderLibrary->setUniformInt("sampleFromHeight", 1);
                shaderLibrary->setUniformInt("material.height", 4);
                glActiveTexture(GL_TEXTURE0 + 4);
                glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            }
            break;
        case ROUGHNESS:
            shaderLibrary->setUniformInt("sampleFromRoughness", 1);
            shaderLibrary->setUniformInt("material.roughness", 5);
            glActiveTexture(GL_TEXTURE0 + 5);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case LIGHTMAP:
            shaderLibrary->setUniformInt("sampleFromLightmap", 1);
            shaderLibrary->setUniformInt("material.lightmap", 6);
            glActiveTexture(GL_TEXTURE0 + 6);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case CUBEMAP:
            shaderLibrary->setUniformInt("cubemap", 5);
            glActiveTexture(GL_TEXTURE0 + 5);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mesh->_textures[i]._id);
            break;
        default:
            break;
        }
    }
}
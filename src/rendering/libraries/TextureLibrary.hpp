#pragma once

// First-party includes
#include "resources/Texture.hpp"
#include "resources/Cubemap.hpp"
#include "resources/Mesh.hpp"

// STD includes
#include <memory>
#include <map>

class FluxLumina;

class TextureLibrary
{
public:
    TextureLibrary(FluxLumina* engine);

    std::shared_ptr<Texture> generate_GL_texture(Texture &texture);
    std::shared_ptr<TextureHDR> generate_GL_textureHDR(TextureHDR& texture);
    std::shared_ptr<Cubemap> generate_GL_cubemap(Cubemap &cubemap, const std::array<Texture, 6>& textures);

    void bindTextures(std::shared_ptr<Mesh> mesh);

private:

    std::map<std::size_t, std::shared_ptr<Texture>> _textures;
    std::map<std::size_t, std::shared_ptr<TextureHDR>> _texturesHDR;
    
    // The engine currently running this manager
    FluxLumina* _ranFrom;
};
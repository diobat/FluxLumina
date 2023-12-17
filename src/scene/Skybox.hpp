#pragma once

// STL headers
#include <memory>

// First-party headers
#include "resources/Cubemap.hpp"

class Skybox
{
public:
    Skybox();

    void setCubemap(std::shared_ptr<Cubemap> cubemap);
    std::shared_ptr<Cubemap> getCubemap() const;

    void setIBLmap(std::shared_ptr<TextureHDR> IBLmap);
    std::shared_ptr<TextureHDR> getIBLmap() const;

    void setIBLcubemap(std::shared_ptr<Cubemap> IBLcubemap);
    std::shared_ptr<Cubemap> getIBLcubemap() const;

private:
    std::shared_ptr<Cubemap> _cubeMap;

    std::shared_ptr<TextureHDR> _IBLmap;
    std::shared_ptr<Cubemap> _IBLcubeMap;
};


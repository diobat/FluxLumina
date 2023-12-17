#include "scene/Skybox.hpp"

Skybox::Skybox()    :
    _cubeMap(nullptr)
{
    ;
}

void Skybox::setCubemap(std::shared_ptr<Cubemap> cubemap)
{
    _cubeMap = cubemap;
}

std::shared_ptr<Cubemap> Skybox::getCubemap() const
{
    return _cubeMap;
}

void Skybox::setIBLmap(std::shared_ptr<TextureHDR> IBLmap)
{
    _IBLmap = IBLmap;
}

std::shared_ptr<TextureHDR> Skybox::getIBLmap() const
{
    return _IBLmap;
}

void Skybox::setIBLcubemap(std::shared_ptr<Cubemap> IBLcubemap)
{
    _IBLcubeMap = IBLcubemap;
}

std::shared_ptr<Cubemap> Skybox::getIBLcubemap() const
{
    return _IBLcubeMap;
}


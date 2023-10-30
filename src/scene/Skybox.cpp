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
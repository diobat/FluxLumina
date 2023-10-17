#include "scene/Skybox.h"

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
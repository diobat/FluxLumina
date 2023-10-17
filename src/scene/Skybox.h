#pragma once

// STD
#include <memory>

// First party
#include "rendering/Cubemap.h"



class Skybox
{
public:
    Skybox();
    
    void rotate();

    void setCubemap(std::shared_ptr<Cubemap> cubemap);
    std::shared_ptr<Cubemap> getCubemap() const;

private:
    std::shared_ptr<Cubemap> _cubeMap;
};


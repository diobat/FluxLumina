#pragma once

// STL headers
#include <memory>

// First-party headers
#include "resources/Cubemap.hpp"

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


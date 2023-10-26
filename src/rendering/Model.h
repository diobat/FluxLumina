#pragma once

// Standard headers
#include <string>
#include <vector>

// First-party headers
#include "Mesh.h"

class Model
{
public:
    
    std::vector<Mesh> meshes;
    bool hasTransparency() const;

    /*  Model Data */
    std::string directory;
};
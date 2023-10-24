#pragma once

// Standard headers
#include <string>
#include <vector>

// First-party headers
#include "Mesh.h"

class Model
{
public:
    /*  Model Data */
    std::string directory;
    std::vector<Mesh> meshes;
    bool hasTransparency = false;
};
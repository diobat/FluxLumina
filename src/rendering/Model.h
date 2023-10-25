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

    bool hasTransparency() const
    {
        for (auto& mesh : meshes)
        {
            if (mesh._hasTransparency)
            {
                return true;
            }
        }
        return false;
    }
};
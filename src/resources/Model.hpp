﻿#pragma once

// STL headers
#include <string>
#include <vector>
#include <memory>

// First-party headers
#include "resources/Mesh.hpp"

class Model
{
public:
    
    std::vector<std::shared_ptr<Mesh>> meshes;
    bool hasTransparency() const;

    /*  Model Data */
    std::string directory;
};
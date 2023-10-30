#pragma once

// STD Headers
#include <vector>
#include <memory>

// First-Party Headers
#include "rendering/shader/Shader.hpp"


class ShaderLibraryContents
{
    friend class ShaderLibrary;
    
public:
    const std::vector<std::shared_ptr<Shader>>& getShaders() const
    {
        return _shaders;
    }

private:
    std::vector<std::shared_ptr<Shader>> _shaders;
};
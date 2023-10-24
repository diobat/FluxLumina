#pragma once

// STL headers
#include <array>

// First-party headers
#include "util/Rotation.h"

struct SceneObjectProperties
{
    std::array<float, 3> coordinates = {0.0f, 0.0f, 0.0f};
    float scale = 1.0f;
    Rotation rotation = Rotation();
};

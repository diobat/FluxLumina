#pragma once

// STL includes
#include <array>

// GLM includes
#include <glm/glm.hpp>

namespace Math
{
    std::array<float, 3> normalize(const std::array<float, 3> &vec);
}

namespace conversion
{
    std::array<float, 3> toFloat3(const glm::vec3& vec);

    glm::vec3 toVec3(const std::array<float, 3> &vec);

}
#pragma once

// STL includes
#include <array>
#include <string>

// Third-party includes
#include <glm/glm.hpp>
#include <boost/functional/hash.hpp>


namespace Math
{
    std::array<float, 3> normalize(const std::array<float, 3> &vec);
    unsigned int nextHighestMultiple(unsigned int value, unsigned int multiple);

    std::size_t calculateHash(const std::string& vec);
}

namespace conversion
{
    std::array<float, 3> toFloat3(const glm::vec3& vec);
    glm::vec3 toVec3(const std::array<float, 3> &vec);
}


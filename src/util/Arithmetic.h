#pragma once

// STL includes
#include <array>

// Third-party includes
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>


namespace Math
{
    std::array<float, 3> normalize(const std::array<float, 3> &vec);
}
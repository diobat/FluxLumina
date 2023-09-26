#include "util/arithmetic.h"


namespace Math
{
    std::array<float, 3> normalize(const std::array<float, 3> &vec)
    {
        std::array<float, 3> result;
        float length = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        result[0] = vec[0] / length;
        result[1] = vec[1] / length;
        result[2] = vec[2] / length;
        return result;        
    }
}
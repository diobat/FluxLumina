#include "util/Arithmetic.hpp"


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

    unsigned int nextHighestMultiple(unsigned int value, unsigned int multiple)
    {
        unsigned int remainder = value % multiple;
        if (remainder == 0)
        {
            return value;
        }
        else
        {
            return value + multiple - remainder;
        }
    }

    std::size_t calculateHash(const std::string& target)
    {
        return boost::hash<std::string>()(target);
    }

}

namespace conversion
{
    std::array<float, 3> toFloat3(const glm::vec3& vec)
    {
        std::array<float, 3> result;
        result[0] = vec.x;
        result[1] = vec.y;
        result[2] = vec.z;
        return result;
    }

    glm::vec3 toVec3(const std::array<float, 3> &vec)
    {
        glm::vec3 result;
        result.x = vec[0];
        result.y = vec[1];
        result.z = vec[2];
        return result;
    }
}
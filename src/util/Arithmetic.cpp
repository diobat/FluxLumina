#include "util/Arithmetic.hpp"

#include "resources/Mesh.hpp"
#include <random>
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


    std::size_t calculateHash(const std::vector<Vertex>& target, const std::vector<unsigned int>& target2)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, calculateHash(target));
        boost::hash_combine(seed, calculateHash(target2));
        return seed;
    }

    std::size_t calculateHash(const std::vector<Vertex>& target)
    {
        std::size_t seed = 0;
        for (const auto& vertex : target)
        {
            boost::hash_combine(seed, vertex.Position.x);
            boost::hash_combine(seed, vertex.Position.y);
            boost::hash_combine(seed, vertex.Position.z);
            boost::hash_combine(seed, vertex.Normal.x);
            boost::hash_combine(seed, vertex.Normal.y);
            boost::hash_combine(seed, vertex.Normal.z);
            boost::hash_combine(seed, vertex.TexCoords.x);
            boost::hash_combine(seed, vertex.TexCoords.y);
            boost::hash_combine(seed, vertex.Color.x);
            boost::hash_combine(seed, vertex.Color.y);
            boost::hash_combine(seed, vertex.Color.z);
            boost::hash_combine(seed, vertex.Tangent.x);
            boost::hash_combine(seed, vertex.Tangent.y);
            boost::hash_combine(seed, vertex.Tangent.z);
        }
        return seed;
    }

    std::size_t calculateHash(const std::vector<unsigned int>& target)
    {
        std::size_t seed = 0;
        for (const auto& index : target)
        {
            boost::hash_combine(seed, index);
        }
        return seed;
    }

    std::vector<glm::vec3> calculateVertexNormals(const std::vector<Vertex>& vertexes, const std::vector<unsigned int>& indices)
    {
        // Calculate normals
        std::vector<glm::vec3> normals(vertexes.size(), glm::vec3(0.0f, 0.0f, 0.0f));
        for (std::size_t i = 0; i < indices.size(); i += 3)
        {
            glm::vec3 v1 = vertexes[indices[i]].Position;
            glm::vec3 v2 = vertexes[indices[i + 1]].Position;
            glm::vec3 v3 = vertexes[indices[i + 2]].Position;

            glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

            normals[indices[i]] += normal;
            normals[indices[i + 1]] += normal;
            normals[indices[i + 2]] += normal;
        }

        // Normalize normals
        for (std::size_t i = 0; i < normals.size(); i++)
        {
            normals[i] = glm::normalize(normals[i]);
        }

        return normals;
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


namespace random
{
    float gFloat(float min, float max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }
}
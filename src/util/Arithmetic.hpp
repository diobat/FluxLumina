#pragma once

// STL includes
#include <array>
#include <vector>
#include <string>

// Third-party includes
#include <glm/glm.hpp>
#include <boost/functional/hash.hpp>

struct Vertex;

namespace Math
{
    std::array<float, 3> normalize(const std::array<float, 3> &vec);
    unsigned int nextHighestMultiple(unsigned int value, unsigned int multiple);

    std::size_t calculateHash(const std::string& vec);

    // Hash function for Vertex
    std::size_t calculateHash(const std::vector<Vertex>& target, const std::vector<unsigned int>& target2);
    std::size_t calculateHash(const std::vector<Vertex>& target);
    std::size_t calculateHash(const std::vector<unsigned int>& target);

    // Calculation of vertex normals in the context of a mesh
    std::vector<glm::vec3> calculateVertexNormals(const std::vector<Vertex>& vertexes, const std::vector<unsigned int>& indices); 
}

namespace conversion
{
    std::array<float, 3> toFloat3(const glm::vec3& vec);
    glm::vec3 toVec3(const std::array<float, 3> &vec);
}

namespace random
{
    float gFloat(float min, float max);
}
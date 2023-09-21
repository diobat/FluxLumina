#include "util/Rotation.h"

namespace
{
    namespace
    {
        const float M_PI = 3.14159265358979323846f;
        const float DEG_TO_RAD = M_PI / 180.0f;

    }

    void normalizeVector(float &x, float &y, float &z)
    {
        float length = sqrt(x * x + y * y + z * z);
        x /= length;
        y /= length;
        z /= length;
    }
}


Rotation::Rotation()
{
    _rotation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Rotation::rotate(float x, float y, float z)
{

    glm::fquat quatx = glm::angleAxis(glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::fquat quaty = glm::angleAxis(glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::fquat quatz = glm::angleAxis(glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::fquat total = quatx * quaty * quatz;

    glm::vec3 axis = glm::vec3(total.x, total.y, total.z);
    float scalar = total.w;
    axis = glm::normalize(axis);


    if (1)   // model space
    {
        _rotation = _rotation * total;
    }
    else                    // world space
        _rotation = total * _rotation;

    _rotation = glm::normalize(_rotation);
}

const glm::fquat& Rotation::getRotation() const
{
    return _rotation;
}
#pragma once

// Standard headers
#include <array>

// Third-party headers
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Rotation
{
public:
    Rotation();
    void rotate(float x, float y, float z);
    const glm::fquat& getRotation() const;
private:
    glm::fquat _rotation;    
};
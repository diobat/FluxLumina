#pragma once

// Project headers
#include "util/Rotation.h"

// Standard headers
#include <array>

class SceneObject
{
public:
    SceneObject();
    virtual ~SceneObject();

    virtual void setPosition(const std::array<float, 3>& coords);
    virtual const std::array<float, 3>& getPosition() const;

    void setScale (float scale);
    float getScale();

    const glm::fquat &getRotation();

    void rotate(float x, float y, float z);

private:
    bool _toRender;

    // Renderable object properties
    std::array<float, 3> _coordinates;
    float _scale;

    std::array<float, 3> _originalRotation;
    Rotation _rotation;

};

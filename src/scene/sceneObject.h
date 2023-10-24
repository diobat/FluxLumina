#pragma once


// Standard headers
#include <array>

// First-party headers
#include "scene/SceneObjectProperties.h"

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

protected:
    // Renderable object properties
    SceneObjectProperties _properties;

private:
    bool _toRender;
    // std::weak_ptr<SceneObject> _parent;
    // std::vector<std::weak_ptr<SceneObject>> _children;
};

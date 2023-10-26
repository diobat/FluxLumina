#pragma once


// Standard headers
#include <array>

// First-party headers
#include "scene/SceneObjectProperties.h"

// Third-party headers
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

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

    bool enabled() const;

    boost::uuids::uuid _id;

protected:
    // Renderable object properties
    SceneObjectProperties _properties;

private:
    
    bool _toRender;
    // std::weak_ptr<SceneObject> _parent;
    // std::vector<std::weak_ptr<SceneObject>> _children;
};

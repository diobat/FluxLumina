#include "scene/SceneObject.h"

namespace
{
    const float M_PI = 3.14159265358979323846f;
    const float DEG_TO_RAD = M_PI / 180.0f;
}

SceneObject::SceneObject()  :
    _toRender(true),
    _properties(),
    _id(boost::uuids::random_generator()())
{
    ;
}

SceneObject::~SceneObject()
{
    ;
}

void SceneObject::setPosition(const std::array<float, 3>& coords)
{
    _properties.coordinates = coords;
}

const std::array<float, 3> &SceneObject::getPosition() const
{
    return _properties.coordinates;
}

void SceneObject::setScale(float scale)
{
    _properties.scale = scale;
}

float SceneObject::getScale()
{
    return _properties.scale;
}

const glm::fquat &SceneObject::getRotation()
{
    return _properties.rotation.getRotation();
}   


void SceneObject::rotate(float x, float y, float z)
{
    _properties.rotation.rotate(x, y, z);
}
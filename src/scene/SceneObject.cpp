#include "scene/SceneObject.hpp"

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

bool SceneObject::enabled() const
{
    return _toRender;
}

boost::uuids::uuid SceneObject::id() const
{
    return _id;
}
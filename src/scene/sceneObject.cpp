#include "scene/SceneObject.h"

SceneObject::SceneObject()  :
    _toRender(true),
    _coordinates({ 0.0f, 0.0f, 0.0f }),
    _scale(1.0f)
{
    ;
}

void SceneObject::setPosition(const std::array<float, 3>& coords)
{
    _coordinates = coords;
}

std::array<float, 3> &SceneObject::getPosition()
{
    return _coordinates;
}

void SceneObject::setScale(float scale)
{
    _scale = scale;
}

float SceneObject::getScale()
{
    return _scale;
}
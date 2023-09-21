#include "scene/SceneObject.h"

namespace
{
    const float M_PI = 3.14159265358979323846f;
    const float DEG_TO_RAD = M_PI / 180.0f;
}

SceneObject::SceneObject()  :
    _toRender(true),
    _coordinates({ 0.0f, 0.0f, 0.0f}),
    _scale(1.0f),
    _rotation(Rotation())
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


const glm::fquat &SceneObject::getRotation()
{
    return _rotation.getRotation();
}   


void SceneObject::rotate(float x, float y, float z)
{
    // Quaternion quatx = Quaternion({x, 1.0f, 0.0f, 0.0f});
    // Quaternion quaty = Quaternion({y, 0.0f, 1.0f, 0.0f});
    // Quaternion quatz = Quaternion({z, 0.0f, 0.0f, 1.0f});

    // Quaternion total = quatz * quaty * quatx;
    // //Quaternion total = quaty * quatx;
    // _rotation = _rotation * total;

    _rotation.rotate(x, y, z);
}
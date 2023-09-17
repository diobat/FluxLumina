#include "scene/SceneObject.h"

void SceneObject::setPosition(const std::array<float, 3>& coords)
{
    _coordinates = coords;
}

std::array<float, 3>& SceneObject::getCoordinates()
{
    return _coordinates;
}

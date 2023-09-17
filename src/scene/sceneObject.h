#pragma once

#include <array>

class SceneObject
{
public:

    void setPosition(const std::array<float, 3>& coords);
    std::array<float, 3>& getCoordinates();

    void kill();

private:
    bool _toRender;
    std::array<float, 3> _coordinates;
};

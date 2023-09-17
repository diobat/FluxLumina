#pragma once

#include <array>

class SceneObject
{
public:
    SceneObject();

    void setPosition(const std::array<float, 3>& coords);
    std::array<float, 3>& getPosition();

    void setScale (float scale);
    float getScale();

    void kill();

private:
    bool _toRender;
    std::array<float, 3> _coordinates;
    float _scale;
};

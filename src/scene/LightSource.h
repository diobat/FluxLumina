#pragma once

// First-party includes
#include <scene/sceneObject.h>
#include "rendering/model.h"

// STD includes
#include <memory>


class LightSource : public SceneObject
{
public:
    LightSource(float intensity = 0.5f, std::array<float, 3> color = {1.0f, 1.0f, 1.0f});

    void setIntensity(float intensity);
    float getIntensity() const;

    void setColor(const std::array<float, 3>& color);
    const std::array<float, 3>& getColor() const;

private:
    float _intensity;
    std::array<float, 3> _color;
};


class AmbientLight
{
public:
    AmbientLight(float intensity = 0.5f, std::array<float, 3> color = {1.0f, 1.0f, 1.0f});

    void setIntensity(float intensity);
    float getIntensity() const;

    void setColor(std::array<float, 3> color);
    const std::array<float, 3>& getColor() const;

private:
    float _intensity;
    std::array<float, 3> _color;
};

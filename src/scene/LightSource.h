#pragma once

// First-party includes
#include <scene/sceneObject.h>


class LightSource : public SceneObject
{
public:
    LightSource(float intensity = 0.5f, std::array<float, 3> color = {1.0f, 1.0f, 1.0f});

    void setIntensity(float intensity);
    float getIntensity();

    void setColor(std::array<float, 3> color);
    std::array<float, 3> getColor();

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

class DiffuseLight : public LightSource
{
public:
    DiffuseLight(float intensity = 0.5f, std::array<float, 3> color = {1.0f, 1.0f, 1.0f}, std::array<float, 3> direction = {0.0f, 0.0f, 0.0f});

    const std::array<float, 3>& getDirection() const;
    void setDirection(std::array<float, 3> direction);

    const std::array<float, 3>& getColor() const;
    void setColor(std::array<float, 3> color);

    float getIntensity() const;
    void setIntensity(float intensity);

private:

    std::array<float, 3> _direction;
    std::array<float, 3> _color;    
    float _intensity;
};

class SpecularLight : public LightSource
{


};

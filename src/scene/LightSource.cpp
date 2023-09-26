#include "scene/LightSource.h"

// Project includes
#include "util/Arithmetic.h"

LightSource::LightSource(float intensity, std::array<float, 3> color)  :
    _intensity(intensity),
    _color(color)
{
    ;
}

AmbientLight::AmbientLight(float intensity, std::array<float, 3> color) : 
    _intensity(intensity),
    _color(color)
{
    ;
}


// Ambient Light
void AmbientLight::setIntensity(float intensity)
{
    _intensity = intensity;
}

float AmbientLight::getIntensity() const
{
    return _intensity;
}

void AmbientLight::setColor(std::array<float, 3> color)
{
    _color = color;
}

const std::array<float, 3>& AmbientLight::getColor() const
{
    return _color;
}


// Diffuse Light
DiffuseLight::DiffuseLight(float intensity, std::array<float, 3> color, std::array<float, 3> direction) :
    _intensity(intensity),
    _color(color)
{
    _direction = Math::normalize(direction);
}

const std::array<float, 3>& DiffuseLight::getDirection() const
{
    return _direction;
}

void DiffuseLight::setDirection(std::array<float, 3> direction)
{
    _direction = direction;
}

const std::array<float, 3>& DiffuseLight::getColor() const
{
    return _color;
}

void DiffuseLight::setColor(std::array<float, 3> color)
{
    _color = color;
}

float DiffuseLight::getIntensity() const
{
    return _intensity;
}

void DiffuseLight::setIntensity(float intensity)
{
    _intensity = intensity;
}


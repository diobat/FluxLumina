#include "scene/LightSource.h"

// Project includes
#include "util/Arithmetic.h"

LightSource::LightSource(float intensity, std::array<float, 3> color)  :
    _intensity(intensity),
    _color(color)
{
    ;
}

void LightSource::setIntensity(float intensity)
{
    _intensity = intensity;
}

float LightSource::getIntensity() const 
{
    return _intensity;
}

void LightSource::setColor(const std::array<float, 3>& color)
{
    _color = color;
}

const std::array<float, 3>& LightSource::getColor() const
{
    return _color;
}

void LightSource::setAttenuationFactors(std::array<float, 3> attenuationFactors)
{
    _attenuationFactors = attenuationFactors;
}

const std::array<float, 3>& LightSource::getAttenuationFactors() const
{
    return _attenuationFactors;
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


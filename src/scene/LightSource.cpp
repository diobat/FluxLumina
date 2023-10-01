#include "scene/LightSource.h"

// Project includes
#include "util/Arithmetic.h"

LightSource::LightSource(float intensity, const std::array<float, 3>& color, const std::array<float, 3>& attenuationFactors)  :
    _intensity(intensity),
    _color(color),
    _attenuationFactors(attenuationFactors)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// Directional Light
////////////////////////////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight(float intensity,const std::array<float, 3>& color) :
    LightSource(intensity, color)
{
    setAttenuationFactors({1.0f, 0.0f, 0.0f});
}

void DirectionalLight::setDirection(std::array<float, 3> direction)
{
    SceneObject::setPosition(direction);
}

const std::array<float, 3>& DirectionalLight::getDirection() const
{
    return SceneObject::getPosition();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Point Light
////////////////////////////////////////////////////////////////////////////////////////////////////

PointLight::PointLight(float intensity,const std::array<float, 3>& color, const std::array<float, 3>& attenuationFactors) :
    LightSource(intensity, color, attenuationFactors)
{
    ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Spot Light
////////////////////////////////////////////////////////////////////////////////////////////////////

SpotLight::SpotLight(float intensity, std::array<float, 3> color,const std::array<float, 3>& attenuationFactors, float cutoff) :
    LightSource(intensity, color),
    _cutoff(cutoff)
{
    _cutoff = 10.0f;
}

void SpotLight::setDirection(std::array<float, 3> direction)
{
    _direction = direction;
}

const std::array<float, 3>& SpotLight::getDirection() const
{
    return _direction;
}

void SpotLight::setCutoff(float cutoff)
{
    _cutoff = std::min(std::max(cutoff, 0.0f), 45.0f);
}


float SpotLight::getCutoff() const
{
    return _cutoff;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Ambient Light
////////////////////////////////////////////////////////////////////////////////////////////////////

AmbientLight::AmbientLight(float intensity, std::array<float, 3> color) : 
    LightSource(intensity, color)
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

const std::array<float, 3> &AmbientLight::getColor() const
{
    return _color;
}

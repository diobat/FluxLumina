#pragma once

// STL headers
#include <memory>

// First-party headers
#include "scene/SceneObject.hpp"
#include "resources/Model.hpp"

enum E_LightType
{
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    SPOT_LIGHT
};  


class LightSource : public SceneObject
{
public:
    LightSource(
        float intensity = 0.8f, 
        const std::array<float, 3>& color = {1.0f, 1.0f, 1.0f}, 
        const std::array<float, 3> &attenuationFactors = {1.0f, 0.0f, 0.0f}
        );

    virtual ~LightSource() = 0{};

    void setIntensity(float intensity);
    float getIntensity() const;

    void setColor(const std::array<float, 3>& color);
    const std::array<float, 3>& getColor() const;

    virtual void setAttenuationFactors(std::array<float, 3> attenuationFactors);
    virtual const std::array<float, 3>& getAttenuationFactors() const;

    float calculateMaxRange() const;

private:
    float _intensity;
    std::array<float, 3> _color;
    std::array<float, 3> _attenuationFactors;
};


class DirectionalLight : public LightSource
{
public:
    DirectionalLight(
        float intensity = 0.8f, 
        const std::array<float, 3>& color = {1.0f, 1.0f, 1.0f}
        );
    ~DirectionalLight() = default;

    // Direction setter and getter just wrap the position setter and getter
    void setDirection(std::array<float, 3> direction);
    const std::array<float, 3>& getDirection() const;
};



class PointLight : public LightSource
{
public:
    PointLight(
        float intensity = 0.8f,
        const std::array<float, 3> &color = {1.0f, 1.0f, 1.0f},
        const std::array<float, 3> &attenuationFactors = {1.0f, 0.0f, 0.0f}
        );
    ~PointLight() = default;
};

class SpotLight : public LightSource
{
public:
    SpotLight(
        float intensity = 0.8f,
        std::array<float, 3> color = {1.0f, 1.0f, 1.0f},
        const std::array<float, 3> &attenuationFactors = { 1.0f, 0.0f, 0.0f },
        const std::array<float, 3>& direction = { 0.0f, -1.0f, 0.0f },
        float cutoff = 10.0f
        );
    ~SpotLight() = default;

    void setDirection(std::array<float, 3> direction);
    const std::array<float, 3>& getDirection() const;
    void pointAt(const std::array<float, 3>& point);

    void setCutoff(float cutoff, float delta = 1.0f);
    const std::array<float, 2>& getCutoff() const;

private:
    std::array<float, 3> _direction;
    std::array<float, 2> _cutoff;

};

class AmbientLight : public LightSource
{
public:
    AmbientLight(float intensity = 0.5f, std::array<float, 3> color = {1.0f, 1.0f, 1.0f});
    ~AmbientLight() = default;

    void setIntensity(float intensity);
    float getIntensity() const;

    void setColor(std::array<float, 3> color);
    const std::array<float, 3>& getColor() const;

private:
    float _intensity;
    std::array<float, 3> _color;
};

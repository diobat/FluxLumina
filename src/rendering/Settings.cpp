#include "rendering/Settings.hpp"

#include "rendering/shader/ShaderLibrary.hpp"

Settings::Settings()    : 
    _shadowQualityGlobal(E_ShadowQuality_Global::HIGH),
    _shadowGlobal(E_Setting::ON),
    _shadowDirectional(E_Setting::ON),
    _shadowPoint(E_Setting::ON),
    _shadowSpot(E_Setting::ON),
    _antiAliasingQuality(E_AntiAliasingQuality::MEDIUM),
    _transparency(E_Setting::ON),
    _gammaCorrection(E_Setting::ON)
{
    ;
}

void Settings::set(E_Settings setting, int value)
{
    switch (setting)
    {
    case E_Settings::SHADOW_QUALITY_GLOBAL:
        _shadowQualityGlobal = static_cast<E_ShadowQuality_Global>(value);
        break;
    case E_Settings::SHADOW_DIRECTIONAL:
        _shadowDirectional = static_cast<E_Setting>(value);
        break;
    case E_Settings::SHADOW_POINT:
        _shadowPoint = static_cast<E_Setting>(value);
        break;
    case E_Settings::SHADOW_SPOT:
        _shadowSpot = static_cast<E_Setting>(value);
        break;
    case E_Settings::ANTI_ALIASING_QUALITY:
        _antiAliasingQuality = static_cast<E_AntiAliasingQuality>(value);
        break;
    case E_Settings::TRANSPARENCY:
        _transparency = static_cast<E_Setting>(value);
        break;
    case E_Settings::GAMMA_CORRECTION:
        _gammaCorrection = static_cast<E_Setting>(value);
        break;
    default:
        break;
    }
}

E_Setting Settings::getShadowGlobal() const
{
    return _shadowGlobal;
}

E_ShadowQuality_Global Settings::getShadowQualityGlobal() const
{
    return _shadowQualityGlobal;
}

E_Setting Settings::getShadowDirectional() const
{
    return _shadowDirectional;
}

E_Setting Settings::getShadowPoint() const
{
    return _shadowPoint;
}

E_Setting Settings::getShadowSpot() const
{
    return _shadowSpot;
}

E_AntiAliasingQuality Settings::getAntiAliasingQuality() const
{
    return _antiAliasingQuality;
}

E_Setting Settings::getTransparency() const
{
    return _transparency;
}

E_Setting Settings::getGammaCorrection() const
{
    return _gammaCorrection;
}
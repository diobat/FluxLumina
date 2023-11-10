#pragma once


enum class E_Settings{SHADOW_QUALITY_GLOBAL, SHADOW_DIRECTIONAL, SHADOW_POINT, SHADOW_SPOT, ANTI_ALIASING_QUALITY, TRANSPARENCY, GAMMA_CORRECTION};

enum class E_Setting{OFF, ON};
enum class E_ShadowQuality_Global{LOW, MEDIUM, HIGH, ULTRA};
enum class E_AntiAliasingQuality{OFF, LOW, MEDIUM, HIGH};


class Settings
{
public:
    Settings();

    void set(E_Settings setting, int value);

    E_Setting getShadowGlobal() const;
    E_ShadowQuality_Global getShadowQualityGlobal() const;
    E_Setting getShadowDirectional() const;
    E_Setting getShadowPoint() const;
    E_Setting getShadowSpot() const;
    E_AntiAliasingQuality getAntiAliasingQuality() const;
    E_Setting getTransparency() const;
    E_Setting getGammaCorrection() const;

private:

    E_ShadowQuality_Global _shadowQualityGlobal;
    E_Setting _shadowGlobal;
    E_Setting _shadowDirectional;
    E_Setting _shadowPoint;
    E_Setting _shadowSpot;
    E_AntiAliasingQuality _antiAliasingQuality;
    E_Setting _transparency;
    E_Setting _gammaCorrection;

};
#include "rendering/Settings.hpp"

// First-party headers
#include "rendering/GLFW_Wrapper.hpp"

#include "rendering/shader/ShaderLibrary.hpp"


Settings::Settings(GLFWwindow* _window)    : 
    _window(_window),
    _shadowQualityGlobal(E_ShadowQuality_Global::HIGH),
    _shadowGlobal(E_Setting::ON),
    _shadowDirectional(E_Setting::ON),
    _shadowPoint(E_Setting::ON),
    _shadowSpot(E_Setting::ON),
    _antiAliasing(E_Setting::ON),
    _transparency(E_Setting::ON),
    _gammaCorrection(E_Setting::ON),
    _faceCulling(E_Setting::ON),
    _depthTest(E_Setting::ON),
    _normalMapping(E_Setting::ON),
    _heightMapping(E_Setting::ON),
    _highDynamicRange(E_Setting::ON)
{
    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
    
    // Initializer list is not enough because some of these 
    // involve also calling some functions
    set(E_Settings::SHADOW_GLOBAL, 1);
    set(E_Settings::ANTI_ALIASING_QUALITY, 1);
    set(E_Settings::TRANSPARENCY, 0);
    set(E_Settings::GAMMA_CORRECTION, 0);
    set(E_Settings::FACE_CULLING, 1);
    set(E_Settings::DEPTH_TEST, 1);
    set(E_Settings::NORMAL_MAPPING, 1);
    set(E_Settings::HEIGHT_MAPPING, 1);
    set(E_Settings::HIGH_DYNAMIC_RANGE, 1);
}

void Settings::set(E_Settings setting, int value)
{
    glfwMakeContextCurrent(_window);

    switch (setting)
    {
    case E_Settings::SHADOW_QUALITY_GLOBAL:
        _shadowQualityGlobal = static_cast<E_ShadowQuality_Global>(value);
        break;
    case E_Settings::SHADOW_GLOBAL:
        _shadowGlobal = static_cast<E_Setting>(value);
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
        _antiAliasing = static_cast<E_Setting>(value);
        if(value)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }
        break;
    case E_Settings::TRANSPARENCY:
        _transparency = static_cast<E_Setting>(value);
        if(value)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        break;
    case E_Settings::GAMMA_CORRECTION:
        _gammaCorrection = static_cast<E_Setting>(value);
        if(value)
        {
            glEnable(GL_FRAMEBUFFER_SRGB); 
        }
        else
        {
            glDisable(GL_FRAMEBUFFER_SRGB); 
        }
        break;
    case E_Settings::FACE_CULLING:
        _faceCulling = static_cast<E_Setting>(value);
        if(value)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        break;
    case E_Settings::DEPTH_TEST:
        _depthTest = static_cast<E_Setting>(value);
        if(value)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        break;
    case E_Settings::NORMAL_MAPPING:
        _normalMapping = static_cast<E_Setting>(value);
        break;
    case E_Settings::HEIGHT_MAPPING:
        _heightMapping = static_cast<E_Setting>(value);
        break;
    case E_Settings::HIGH_DYNAMIC_RANGE:
        _highDynamicRange = static_cast<E_Setting>(value);
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

E_Setting Settings::getAntiAliasing() const
{
    return _antiAliasing;
}

E_Setting Settings::getTransparency() const
{
    return _transparency;
}

E_Setting Settings::getGammaCorrection() const
{
    return _gammaCorrection;
}

E_Setting Settings::getFaceCulling() const
{
    return _faceCulling;
}

E_Setting Settings::getDepthTest() const
{
    return _depthTest;
}

E_Setting Settings::getNormalMapping() const
{
    return _normalMapping;
}

E_Setting Settings::getHeightMapping() const
{
    return _heightMapping;
}

E_Setting Settings::getHighDynamicRange() const
{
    return _highDynamicRange;
}
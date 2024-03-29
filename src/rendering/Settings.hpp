#pragma once

class GLFWwindow;

enum class E_Settings{SHADOW_QUALITY_GLOBAL,SHADOW_GLOBAL, SHADOW_DIRECTIONAL, SHADOW_POINT, SHADOW_SPOT, ANTI_ALIASING_QUALITY, TRANSPARENCY, GAMMA_CORRECTION, FACE_CULLING, DEPTH_TEST, NORMAL_MAPPING, HEIGHT_MAPPING, HIGH_DYNAMIC_RANGE, BLOOM, SSAO, SEAMLESS_CUBEMAP_SAMPLING, VSYNC, POLYGON_LINES, GRAPHICAL_DEBUG_OUTPUT};

enum class E_Setting{OFF, ON};
enum class E_ShadowQuality_Global{LOW, MEDIUM, HIGH, ULTRA};
enum class E_PolygonMode{FILL, LINES, POINTS};

class Settings
{
public:
    Settings(GLFWwindow* _window);

    void set(E_Settings setting, int value);

    E_Setting getShadowGlobal() const;
    E_ShadowQuality_Global getShadowQualityGlobal() const;
    E_Setting getShadowDirectional() const;
    E_Setting getShadowPoint() const;
    E_Setting getShadowSpot() const;
    E_Setting getAntiAliasing() const;
    E_Setting getTransparency() const;
    E_Setting getGammaCorrection() const;
    E_Setting getFaceCulling() const;
    E_Setting getDepthTest() const;
    E_Setting getNormalMapping() const;
    E_Setting getHeightMapping() const;
    E_Setting getHighDynamicRange() const;
    E_Setting getBloom() const;
    E_Setting getSSAO() const;
    E_Setting getSeamlessCubemapSampling() const;
    E_Setting getvSync() const;
    E_PolygonMode getPolygonMode() const;
    E_Setting getGLDebugOutput() const;

private:
    GLFWwindow* _window;

    E_ShadowQuality_Global _shadowQualityGlobal;
    E_Setting _shadowGlobal;
    E_Setting _shadowDirectional;
    E_Setting _shadowPoint;
    E_Setting _shadowSpot;
    E_Setting _antiAliasing;
    E_Setting _transparency;
    E_Setting _gammaCorrection;
    E_Setting _faceCulling;
    E_Setting _depthTest;
    E_Setting _normalMapping;
    E_Setting _heightMapping;
    E_Setting _highDynamicRange;
    E_Setting _bloom;
    E_Setting _ssao;
    E_Setting _seamlessCubemapSampling;
    E_Setting _vSync;
    E_PolygonMode _polygonMode;
    E_Setting _graphicalDebugOutput;
    
};
#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

//First party headers
#include "scene/Scene.hpp"
#include "rendering/shader/ShaderLibrary.hpp"

class LightLibrary
{
public:
    bool prepare(ShaderLibrary& shaders, const LightContents& lights);

private:
	void lightSetup(ShaderLibrary& shaders, unsigned int lightIndex, const DirectionalLight &light);
	void lightSetup(ShaderLibrary& shaders, unsigned int lightIndex, const PointLight &light);
	void lightSetup(ShaderLibrary& shaders, unsigned int lightIndex, const SpotLight &light);
};	
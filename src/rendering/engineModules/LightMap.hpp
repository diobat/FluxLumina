#pragma once

// GLM includes
#include <glm/glm.hpp>

// STL includes
#include <array>
#include <memory>

// First party includes
#include "resources/Texture.hpp"
#include "rendering/framebuffer/FBO.hpp"

class LightLibrary;

class LightMap
{
public:
	LightMap(LightLibrary* library);
	void init(unsigned int size = 512u, unsigned int scale = 4u);

	unsigned int bakeFromTexture(std::shared_ptr<TextureHDR> texture);
	
	unsigned int PBR_Diffuse_convoluteLightMap();
	unsigned int PBR_Specular_convoluteLightMap();
	unsigned int PBR_Specular_BRDF_LUT();

	std::shared_ptr<FBO> getLightMapFBO() const;
	std::shared_ptr<FBO> getDiffuseIrradianceFBO() const;
	std::shared_ptr<FBO> getSpecularPreFilterFBO() const;
	std::shared_ptr<FBO> getSpecularBRDFLUT() const;

private:
	std::array<glm::mat4, 6> _viewMatrices;
	std::shared_ptr<FBO> _lightMapFBO;
	std::shared_ptr<FBO> _diffuseIrradianceFBO;
	std::shared_ptr<FBO> _specularPreFilterFBO;
	std::shared_ptr<FBO> _specularBRDFLUT;

	unsigned int _scale;
	unsigned int _size;
	LightLibrary* _library;
};
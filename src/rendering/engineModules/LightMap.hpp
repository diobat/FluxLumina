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
	void init();

	unsigned int bakeFromTexture(std::shared_ptr<TextureHDR> texture);

private:
	std::array<glm::mat4, 6> _viewMatrices;
	std::shared_ptr<FBO> _lightMapFBO;

	LightLibrary* _library;
};
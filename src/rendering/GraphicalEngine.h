#pragma once

// First-party includes
#include "rendering\mesh.h"
#include "rendering\texture.h"
#include "scene/Scene.h"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeMesh(Mesh &mesh) = 0;
	virtual void initializeTexture(Texture &texture) = 0;
	virtual void initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures) = 0;

	virtual void renderFrame(std::shared_ptr<Scene>) = 0{};

	void bindScene(std::shared_ptr<Scene> scene);
	void unbindScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> getScene(unsigned int sceneIndex) const;

	virtual unsigned int getShaderProgramID(unsigned int shaderIndex) = 0;

protected:
	std::vector<std::shared_ptr<Scene>> _scenes;
};



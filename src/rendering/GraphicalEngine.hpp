#pragma once

// First-party includes
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"
#include "scene/Scene.hpp"
#include "rendering/MeshLibrary.hpp"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeMesh(std::shared_ptr<Mesh> &mesh) = 0;
	virtual void initializeTexture(Texture &texture) = 0;
	virtual void initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures) = 0;

	virtual void renderFrame(std::shared_ptr<Scene>) = 0{};

	void bindScene(std::shared_ptr<Scene> scene);
	void unbindScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> getScene(unsigned int sceneIndex) const;

	virtual unsigned int getShaderProgramID(unsigned int shaderIndex) = 0;

	void bindMeshLibrary(std::shared_ptr<MeshLibrary> meshLibrary);

protected:
	std::vector<std::shared_ptr<Scene>> _scenes;
	std::shared_ptr<MeshLibrary> _meshLibrary;
};



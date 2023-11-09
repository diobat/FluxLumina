#pragma once

// First-party includes
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"
#include "scene/Scene.hpp"
#include "rendering/MeshLibrary.hpp"

#include "rendering/strategy/StrategyChain.hpp"
#include "rendering/shader/ShaderLibrary.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeMesh(std::shared_ptr<Mesh> &mesh) = 0;
	virtual void initializeTexture(Texture &texture) = 0;
	virtual void initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures) = 0;
	virtual void bindTextures(std::shared_ptr<Mesh> mesh) = 0;

	virtual void renderFrame(std::shared_ptr<Scene>) = 0{};
	virtual void renderModel(ModelObject& model) = 0;
	virtual void renderInstancedMeshes(std::shared_ptr<InstancingManager> instanceManager) = 0;
	virtual void renderSkybox(Skybox& skybox) = 0;

	void bindScene(std::shared_ptr<Scene> scene);
	void unbindScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> getScene(unsigned int sceneIndex = 0) const;

	virtual unsigned int getShaderProgramID(unsigned int shaderIndex) = 0;

	void bindMeshLibrary(std::shared_ptr<MeshLibrary> meshLibrary);

	std::shared_ptr<FBOManager> getFBOManager() { return _frameBuffers; };
	std::shared_ptr<ShaderLibrary> getShaderLibrary() { return _shaderPrograms; };
	std::shared_ptr<LightLibrary> getLightLibrary() { return _lightLibrary; };
	std::shared_ptr<InstancingManager> getInstancingManager() { return _instancingManager; }

	std::array<int, 2> getViewportSize() const { return { _viewportWidth, _viewportHeight }; }

protected:
	int _viewportWidth, _viewportHeight;

	std::vector<std::shared_ptr<Scene>> _scenes;
	std::shared_ptr<MeshLibrary> _meshLibrary;

	// Framebuffer
	std::shared_ptr<FBOManager> _frameBuffers;

	// Shaders
	std::shared_ptr<ShaderLibrary> _shaderPrograms;

	// Lights
	std::shared_ptr<LightLibrary> _lightLibrary;

	// Instancing
	std::shared_ptr<InstancingManager> _instancingManager;

	// Rendering strategy
	std::shared_ptr<StrategyChain> _strategyChain;
};



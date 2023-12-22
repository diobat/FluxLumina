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
#include "rendering/Settings.hpp"

class GraphicalEngine
{
public:
	GraphicalEngine();
	
	virtual void initializeTexture(Texture &texture) = 0;
	virtual void initializeTextureHDR(TextureHDR& texture) = 0;
	virtual void initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures) = 0;
	virtual void bindTextures(std::shared_ptr<Mesh> mesh) = 0;

	virtual void renderFrame(std::shared_ptr<Scene>) = 0{};
	virtual void renderModel(ModelObject& model) = 0;
	virtual void renderInstancedMeshes(std::shared_ptr<InstancingManager> instanceManager) = 0;
	virtual void renderSkybox(Skybox& skybox) = 0;

	void bindScene(std::shared_ptr<Scene> scene);
	void unbindScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> getScene(unsigned int sceneIndex = 0) const;
	std::vector<std::shared_ptr<Scene>> getScenes() const { return _scenes; }

	void bindMeshLibrary(std::shared_ptr<MeshLibrary> meshLibrary);

	std::shared_ptr<FBOManager> getFBOManager() { return _frameBuffers; };
	std::shared_ptr<ShaderLibrary> getShaderLibrary() { return _shaderPrograms; };
	std::shared_ptr<LightLibrary> getLightLibrary() { return _lightLibrary; };
	std::shared_ptr<InstancingManager> getInstancingManager() { return _instancingManager; }
	std::shared_ptr<StrategyChain> getStrategyChain() { return _strategyChain; }
	std::shared_ptr<Settings> getSettings() const { return _settings; }

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

	// Engine settings
	std::shared_ptr<Settings> _settings;
};



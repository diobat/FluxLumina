#pragma once

#include <memory>
#include <vector>
#include <array>

class Scene;
class SceneObjectFactory;
class MeshLibrary;
class TextureLibrary;
class FBOManager;
class ShaderLibrary;
class LightLibrary;
class InstancingManager;
class StrategyChain;
class Settings;
class glfwKeyboardScanner;

class GraphicalEngine
{
public:
	GraphicalEngine();	

	virtual void renderFrame(std::shared_ptr<Scene>) = 0;

	void bindScene(std::shared_ptr<Scene> scene);
	void unbindScene(std::shared_ptr<Scene> scene);
	std::shared_ptr<Scene> getScene(unsigned int sceneIndex = 0) const;
	std::vector<std::shared_ptr<Scene>> getScenes() const { return _scenes; }

	std::shared_ptr<SceneObjectFactory> getSceneObjectFactory() { return _sceneObjectFactory; };
	std::shared_ptr<MeshLibrary> getMeshLibrary() { return _meshLibrary; };
	std::shared_ptr<TextureLibrary> getTextureLibrary() { return _textureLibrary; };
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

	// SceneObjectFactory
	std::shared_ptr<SceneObjectFactory> _sceneObjectFactory;

	// Meshes
	std::shared_ptr<MeshLibrary> _meshLibrary;

	// Textures
	std::shared_ptr<TextureLibrary> _textureLibrary;

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

	// User Inputs
	std::shared_ptr<glfwKeyboardScanner> _userInput;

};



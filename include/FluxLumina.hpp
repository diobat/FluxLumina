#pragma once

// std includes
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <functional>

// Third party includes
#include "boost/uuid/uuid.hpp"

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
struct GLFWwindow;

enum class E_RenderStrategy : unsigned int
{
    ForwardShading,
    DeferredShading,
    PBSShading
};

class FluxLumina
{
public:
	FluxLumina(E_RenderStrategy strategy, const std::string& shaderPath = "");

	int initialize(E_RenderStrategy strategy, const std::string& shaderPath);

	// Update the engine internal state, and render a frame. 
	void update();	// This is an indefinitely blocking call.

	// Adds a model to the bound scene
	boost::uuids::uuid create_Model(
		const std::string &modelPath, 
		const std::string& shader = "Basic", 
		bool flipUVs = false, 
		std::array<std::vector<std::string>, 2> textureLocations = {}
		);

	boost::uuids::uuid create_Model(
		const std::vector<std::array<float, 3>>& vertices,
		const std::vector<unsigned int>& indices,
		const std::vector<std::array<float, 3>>& colors,
		const std::string& shader = "Basic"
		);

	// Add lightsource to the scene
	boost::uuids::uuid create_LightSource(unsigned int type);

	// Create camera
	void create_Camera(float fov = 70.0f, float translationSpeed = 0.2f, float rotationSpeed = 0.001f);

	// Create skybox
	void create_Skybox(const std::vector<std::string>& path);

	// Create HDR texture
	unsigned int create_IBL(const std::string& path, bool flipUVs = false);

	// Sets values for a SceneObject
	void setPosition(boost::uuids::uuid modelID, std::array<float, 3> position);
	void setRotation(boost::uuids::uuid modelID, std::array<float, 3> rotation);
	void setScale(boost::uuids::uuid modelID, float scale);
	void setEnabled(boost::uuids::uuid modelID, bool enabled);

	// Values for a camera
	void setCameraPosition(std::array<float, 3> position);
	std::array<float, 3> getCameraPosition() const;
	void setCameraRotation(std::array<float, 2> rotation);

	// Sets values for a LightSource
	void setColor(boost::uuids::uuid lightID, std::array<float, 3> color);
	void setAttenuationFactors(boost::uuids::uuid lightID, std::array<float, 3> attenuationFactors);
	void setDirection(boost::uuids::uuid lightID, std::array<float, 3> direction);
	void setSpotlightRadius(boost::uuids::uuid lightID, float radius);

	// Bound scenes management
	std::shared_ptr<Scene> getScene(unsigned int sceneIndex = 0) const;
	void bindScene(std::shared_ptr<Scene> scene);
	void unbindScene(std::shared_ptr<Scene> scene);

	// Viewport
	std::array<int, 2> getViewportSize() const { return { _viewportWidth, _viewportHeight }; }

	// User Input
	bool bindUserInput(int key, std::function<void()> callback);

	// Module getters
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

private:
	// Render a frame by applying the loaded trategy
	void renderFrame(std::shared_ptr<Scene> scene);
	// Window
	void resizeWindowCallback(GLFWwindow *window, int width, int height);

	int _viewportWidth, _viewportHeight;

	std::vector<std::shared_ptr<Scene>> _scenes;					// Contains bound scenes
	std::shared_ptr<SceneObjectFactory> _sceneObjectFactory;		// Creates scene objects
	std::shared_ptr<MeshLibrary> _meshLibrary;						// Owns all meshes
	std::shared_ptr<TextureLibrary> _textureLibrary;				// Creates and owns all textures
	std::shared_ptr<FBOManager> _frameBuffers;						// Creates and owns all framebuffers
	std::shared_ptr<ShaderLibrary> _shaderPrograms;					// Creates and owns all shaders
	std::shared_ptr<LightLibrary> _lightLibrary;					// Owns all lights
	std::shared_ptr<InstancingManager> _instancingManager;			// Manages instancing
	std::shared_ptr<StrategyChain> _strategyChain;					// Manages rendering strategies
	std::shared_ptr<Settings> _settings;							// Handles settings
	std::shared_ptr<glfwKeyboardScanner> _userInput;				// Handles user input
	
	// GLFW window
	GLFWwindow* _window;
};
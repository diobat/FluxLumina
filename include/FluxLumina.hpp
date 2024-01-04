#pragma once

#include <string>
#include <array>
#include <vector>

// First party includes
#include "GraphicalEngine.hpp"

// Third party includes
#include "boost/uuid/uuid.hpp"


class GLFWwindow;

enum class E_RenderStrategy : unsigned int
{
    ForwardShading,
    DeferredShading,
    PBSShading
};

class FluxLumina : public GraphicalEngine
{
public:
	FluxLumina(E_RenderStrategy strategy);

	int initialize(E_RenderStrategy strategy);

	// Update the engine internal state, and render a frame. This is an indefinitely blocking call.
	void update();

	// Adds a model to the bound scene
	boost::uuids::uuid create_Model(
		const std::string &modelPath, 
		const std::string& shader = "Basic", 
		bool flipUVs = false, 
		std::array<std::vector<std::string>, 2> textureLocations = {}
		);

	// Add lightsource to the scene
	boost::uuids::uuid create_LightSource(unsigned int type);

	// Create camera
	void create_Camera();

	// Create skybox
	void create_Skybox(const std::vector<std::string>& path);

	// Create HDR texture
	unsigned int create_IBL(const std::string& path, bool flipUVs = false);

	// Sets values for a SceneObject
	void setPosition(boost::uuids::uuid modelID, std::array<float, 3> position);
	void setRotation(boost::uuids::uuid modelID, std::array<float, 3> rotation);
	void setScale(boost::uuids::uuid modelID, float scale);

	// Sets values for a LightSource
	void setColor(boost::uuids::uuid lightID, std::array<float, 3> color);
	void setAttenuationFactors(boost::uuids::uuid lightID, std::array<float, 3> attenuationFactors);
	void setDirection(boost::uuids::uuid lightID, std::array<float, 3> direction);
	void setSpotlightRadius(boost::uuids::uuid lightID, float radius);

private:
	// Render a frame by applying the loaded trategy
	void renderFrame(std::shared_ptr<Scene> scene) override;
	// Window
	void resizeWindowCallback(GLFWwindow *window, int width, int height);

	// GLFW window
	GLFWwindow* _window;

};
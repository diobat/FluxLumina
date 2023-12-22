#pragma once

// STD library includes
#include <iostream>

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// First party includes
#include "rendering/GraphicalEngine.hpp"
#include "rendering/shader/ShaderLibrary.hpp"
#include "resources/Texture.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "user_input/glfwUserInputScanner.hpp"
#include "util/Arithmetic.hpp"

class openGL : public GraphicalEngine
{
public:

	int initialize(GLFWwindow* window, E_RenderStrategy strategy = E_RenderStrategy::ForwardShading);

	// Render a frame by applying the loaded strategy
	void renderFrame(std::shared_ptr<Scene> scene) override;

	GLFWwindow* getWindowPtr();

	// Models
	void bindTextures(std::shared_ptr<Mesh> mesh) override;
	void renderModel(ModelObject &model) override;
	void renderInstancedMeshes(std::shared_ptr<InstancingManager> instancingManager = nullptr) override;
	void renderSkybox(Skybox &skybox) override;

private:
	// Window
	void resizeWindowCallback(GLFWwindow *window, int width, int height);

	// GLFW window
	GLFWwindow* _window;

};
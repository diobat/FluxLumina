#pragma once

// STD library includes
#include <iostream>

// GLFW incldues
#include "rendering/GLFW_Wrapper.h"

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// First party includes
#include "rendering/GraphicalEngine.h"
#include "rendering/Shader.h"
#include "rendering/texture.h"
#include "user_input/glfwUserInputScanner.h"
#include "util\listenerPattern.h"

class openGL : public GraphicalEngine//, public I_Listener
{
public:

	openGL();
	~openGL();

	int initialize();

	void bindScene(std::shared_ptr<Scene> scene);
	void renderFrame();
	void resizeWindow(GLFWwindow* window, int width, int height);

	GLFWwindow* getWindowPtr();

	// Listener overrides
	// void callback(E_EventType event = E_EventType::NoEvent, std::any *object = nullptr);

	// GraphicalEngine overrides
	void registerObject(SceneObject &object);
	void unregisterObject(SceneObject &object);

private:
	void drawSingleModel();


	GLFWwindow* _window;
	std::shared_ptr<Scene> _scene;
	std::shared_ptr<UserInput::glfwKeyboardScanner> _userInput;

	std::shared_ptr<Camera> _camera;
	std::vector<Shader> _shaderPrograms;
	std::vector<Texture> _textures;


};
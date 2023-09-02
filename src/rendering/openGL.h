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
#include "user_input/glfwUserInputScanner.h"

class openGL : public GraphicalEngine
{
public:

	openGL();
	~openGL();

	int initialize();

	void bindScene(std::shared_ptr<Scene> scene);

	void renderFrame();

	void resizeWindow(GLFWwindow* window, int width, int height);

	GLFWwindow* getWindowPtr();

private:

	GLFWwindow* _window;
	std::shared_ptr<Scene> _scene;
	std::shared_ptr<UserInput::glfwKeyboardScanner> _userInput;

	std::shared_ptr<Camera> _camera;
	std::vector<Shader> _shaderPrograms;
	std::vector<Texture> _textures;


};
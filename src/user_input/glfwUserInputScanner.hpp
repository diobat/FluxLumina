#pragma once

// STD library includes
#include <memory>
#include <map>


// First-party includes
#include "scene/Scene.hpp"


class GLFWwindow;

class glfwKeyboardScanner
{
public:
	glfwKeyboardScanner(GLFWwindow* window);
	void tickCallback();
	void bindToScene(std::shared_ptr<Scene> scene);
	void readInputs();

private:
	GLFWwindow* _window;
	std::shared_ptr<Scene> _scene;
};




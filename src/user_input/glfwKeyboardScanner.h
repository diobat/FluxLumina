#pragma once

// STD library includes
#include <memory>

// GLFW incldues
#include "rendering/GLFW_Wrapper.h"

// First-party includes
#include "scene/Scene.h"



namespace UserInput
{

	class glfwKeyboardScanner
	{
	public:
		glfwKeyboardScanner(GLFWwindow* window);

		//void bindToInput();
		void bindToScene(std::shared_ptr<Scene> scene);
		//void callback(int key, int scancode, int action, int mods);

	private:
		GLFWwindow* _window;
	};

}



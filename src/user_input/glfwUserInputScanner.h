#pragma once

// STD library includes
#include <memory>
#include <map>

// GLFW incldues
#include "rendering/GLFW_Wrapper.h"

// First-party includes
#include "scene/Scene.h"
#include "util/tickHandler.h"

namespace UserInput
{

	class glfwKeyboardScanner : public Utils::Ticker::Handler
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
}



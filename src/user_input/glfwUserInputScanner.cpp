#include "user_input/glfwUserInputScanner.h"

//Because openGL runs in C code it has no idea what classes and this-> are.
//Its much more convoluted to implement callbacks as non-member functions but 
//I have little choice. 
namespace UserInput
{
	bool debugMode = false;

	GLFWwindow* renderWindow;
	std::shared_ptr<Scene> boundScene;
	std::map<int, bool> keyMap;
	bool captureMouse = true;

	std::array<double, 2> lastMousePos = {0.0, 0.0};

	void resetCursorPos()
	{
		int width, height;
		glfwGetWindowSize(renderWindow, &width, &height);
		glfwSetCursorPos(renderWindow, width / 2, height / 2);
		glfwSetCursorPos(renderWindow, lastMousePos[0], lastMousePos[1]);
	}

	void setLastMousePosToCurrent()
	{
		glfwGetCursorPos(renderWindow, &lastMousePos[0], &lastMousePos[1]);
	}

	void toggleMouseMode()
	{
		captureMouse = !captureMouse;
		std::cout << "Mouse mode toggled: " << captureMouse << std::endl;

		if (captureMouse)
		{
			glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			setLastMousePosToCurrent();
		}
		else
		{
			glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

	}

	void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		keyMap[key] = action;

		if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		{
			toggleMouseMode();
		}

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (!captureMouse)	// We only capture input if the setting in toggled
		{
			return;
		}

		float xPos_delta = xpos - lastMousePos[0];
		float yPos_delta = ypos - lastMousePos[1];

		if(debugMode)
		{
			std::cout << "Horizontal: " << xPos_delta << std::endl;
			std::cout << "Vertical: " << yPos_delta << std::endl;
		}

		// Get and update the camera
		Camera& cam = *boundScene->getActiveCamera();
		cam.addRotationDelta(std::array<float, 2>{static_cast<float>(xPos_delta), static_cast<float>(yPos_delta)});

		lastMousePos = {xpos, ypos};
	}
}

using namespace UserInput;

glfwKeyboardScanner::glfwKeyboardScanner(GLFWwindow* window) :
	_window(window)
{
	renderWindow = window;

	// Keyboard callbacks
	glfwSetKeyCallback(_window, keyboardCallback);
	// Mouse callbacks
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	resetCursorPos();
}

void glfwKeyboardScanner::tickCallback()
{
	readInputs();
}

void glfwKeyboardScanner::bindToScene(std::shared_ptr<Scene> scene)
{
	boundScene = scene;
}

void glfwKeyboardScanner::readInputs()
{
	for (std::pair<int, bool> key : keyMap)
	{
		if (key.second != GLFW_RELEASE)
		{
			std::shared_ptr<Camera>& cam = boundScene->getActiveCamera();
			switch (key.first)
			{ 
				case GLFW_KEY_W:
					cam->move(relativeDirections::FORWARD);
					break;
				case GLFW_KEY_A:
					cam->move(relativeDirections::LEFT);
					break;
				case GLFW_KEY_S:
					cam->move(relativeDirections::BACKWARD);
					break;
				case GLFW_KEY_D:
					cam->move(relativeDirections::RIGHT);
					break;
				case GLFW_KEY_Q:
					cam->move(relativeDirections::UP);
					break;
				case GLFW_KEY_E:
					cam->move(relativeDirections::DOWN);
					break;
			}

		}



	}
}

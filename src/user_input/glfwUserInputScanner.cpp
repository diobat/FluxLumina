#include "user_input/glfwUserInputScanner.h"



//Because openGL runs in C code it has no idea what classes and this-> are.
//Its much more convoluted to implement callbacks as non-member functions but 
//I have little choice. The alternative is a mess
namespace UserInput
{
	GLFWwindow* renderWindow;
	std::shared_ptr<Scene> boundScene;
	std::map<int, bool> keyMap;
	bool captureMouse = true;

	void resetCursorPos()
	{
		int width, height;
		glfwGetWindowSize(renderWindow, &width, &height);
		glfwSetCursorPos(renderWindow, width / 2, height / 2);
	}

	void toggleMouseMode()
	{
		captureMouse = !captureMouse;
		std::cout << "Mouse mode toggled: " << captureMouse << std::endl;

		if (captureMouse)
		{
			resetCursorPos();
			glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if (!captureMouse)
		{
			return;
		}
		std::cout << "Horizontal: " << xpos << std::endl;
		std::cout << "Vertical: " << ypos << std::endl;
		Camera& cam = boundScene->getActiveCamera();
		cam.updateRotation(std::array<float, 2>{static_cast<float>(xpos), static_cast<float>(ypos)});
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
			Camera& cam = boundScene->getActiveCamera();
			switch (key.first)
			{ 
				case GLFW_KEY_W:
					cam.move(relativeDirections::FORWARD);
					break;
				case GLFW_KEY_A:
					cam.move(relativeDirections::LEFT);
					break;
				case GLFW_KEY_S:
					cam.move(relativeDirections::BACKWARD);
					break;
				case GLFW_KEY_D:
					cam.move(relativeDirections::RIGHT);
					break;
				case GLFW_KEY_Q:
					cam.move(relativeDirections::UP);
					break;
				case GLFW_KEY_E:
					cam.move(relativeDirections::DOWN);
					break;
			}

		}



	}
}


//void glfwKeyboardScanner::updateCamera()
//{
//	Camera& cam = boundScene->getActiveCamera();
//	for (std::pair<const int, bool>& pair : keyMap)
//	{
//		if (pair.second)
//		{
//			switch (pair.first)
//			{
//			case GLFW_KEY_W:
//				cam.move(relativeDirections::FORWARD);
//				break;
//			case GLFW_KEY_A:
//				cam.move(relativeDirections::LEFT);
//				break;
//			case GLFW_KEY_S:
//				cam.move(relativeDirections::BACKWARD);
//				break;
//			case GLFW_KEY_D:
//				cam.move(relativeDirections::RIGHT);
//				break;
//			case GLFW_KEY_Q:
//				cam.move(relativeDirections::UP);
//				break;
//			case GLFW_KEY_E:
//				cam.move(relativeDirections::DOWN);
//				break;
//
//				break;
//			}
//		}
//	}
//}
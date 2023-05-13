#include "user_input/glfwKeyboardScanner.h"

//Because openGL runs in C code it has no idea what classes and this-> are.
//Its much more convoluted to implement callbacks as non-member functions but 
//I have little choice. The alternative is a mess
namespace UserInput
{
	std::shared_ptr<Scene> boundScene;
	std::map<int, bool> keyMap;

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		std::cout << "Keyboard callback called " << std::endl;


		Camera& cam = boundScene->getActiveCamera();

		keyMap[key] = (action == GLFW_PRESS);

		switch (key)
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


using namespace UserInput;

glfwKeyboardScanner::glfwKeyboardScanner(GLFWwindow* window) :
	_window(window)
{

	//// Window resize code
	//glfwSetWindowUserPointer(_window, this);

	//auto func = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	//{
	//	static_cast<glfwKeyboardScanner*>(glfwGetWindowUserPointer(window))->callback(key, scancode, action, mods);
	//};

	glfwSetKeyCallback(_window, keyboardCallback);

}

void glfwKeyboardScanner::bindToScene(std::shared_ptr<Scene> scene)
{
	boundScene = scene;
}

//void glfwKeyboardScanner::callback(int key, int scancode, int action, int mods)
//{
//	std::cout << "Keyboard callback called " << std::endl;
//	Camera& cam = _scene->getActiveCamera();
//
//	keyMap[key] = (action == GLFW_PRESS);
//
//	switch (key)
//	{
//
//	case GLFW_KEY_W:
//		cam.move(relativeDirections::FORWARD);
//		break;
//	case GLFW_KEY_A:
//		cam.move(relativeDirections::LEFT);
//		break;
//	case GLFW_KEY_S:
//		cam.move(relativeDirections::BACKWARD);
//		break;
//	case GLFW_KEY_D:
//		cam.move(relativeDirections::RIGHT);
//		break;
//	case GLFW_KEY_Q:
//		cam.move(relativeDirections::UP);
//		break;
//	case GLFW_KEY_E:
//		cam.move(relativeDirections::DOWN);
//		break;
//
//	}
//
//}
//
//

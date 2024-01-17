#pragma once

// STD library includes
#include <memory>
#include <unordered_map>

// First-party includes
#include "scene/Scene.hpp"

struct GLFWwindow;

class glfwKeyboardScanner
{
public:
	glfwKeyboardScanner(GLFWwindow* window);
	void interruptCallback(int key);
	void bindToScene(std::shared_ptr<Scene> scene);

	bool bindKey(int key, std::function<void()> callback);
	void unbindKey(int key);

private:
	GLFWwindow* _window;

	std::unordered_map<int, std::function<void()>> callbackMap;
};




#pragma once

// First party includes
#include "rendering/GraphicalEngine.hpp"
#include "rendering/strategy/StrategyChain.hpp"

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

class openGL : public GraphicalEngine
{
public:
	openGL(std::shared_ptr<Scene> scene , E_RenderStrategy strategy);

	int initialize(std::shared_ptr<Scene> scene , E_RenderStrategy strategy);

	// Update the engine internal state
	void update();

private:
	// Render a frame by applying the loaded trategy
	void renderFrame(std::shared_ptr<Scene> scene) override;
	// Window
	void resizeWindowCallback(GLFWwindow *window, int width, int height);

	// GLFW window
	GLFWwindow* _window;

};
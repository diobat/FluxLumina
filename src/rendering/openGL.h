#pragma once


// STD library includes
#include <iostream>

// GLFW incldues
//#define ASSIMP_USE_HUNTER
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// First party includes
#include "rendering/GraphicalEngine.h"
#include "rendering/Shader.h"
#include "scene/Scene.h"




class openGL : public GraphicalEngine
{
public:

	openGL();

	int initialize();

	void renderFrame();

	void resizeWindow(GLFWwindow* window, int width, int height);


private:

	GLFWwindow* _window;

	std::shared_ptr<Scene> _scene;
	std::shared_ptr<Camera> _camera;
	std::vector<Shader> _shaderPrograms;

};
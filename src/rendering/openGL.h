#pragma once

// STD library includes
#include <iostream>

// GLFW incldues
#include "rendering/GLFW_Wrapper.h"

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// First party includes
#include "rendering/GraphicalEngine.h"
#include "rendering/Shader.h"
#include "rendering/texture.h"
#include "user_input/glfwUserInputScanner.h"
#include "util/Arithmetic.h"

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

	void initializeMesh(Mesh& mesh);
	void initializeTexture(Texture& texture);
	void bindTextures(Mesh &mesh);
	void renderModel(ModelObject &model);

	unsigned int getShaderProgramID(unsigned int shaderIndex) const;
	void useShader(unsigned int shaderIndex);

private:
	void allLightsSetup(const LightContents &lights);
	void lightSetup(unsigned int shaderIndex, const DirectionalLight &light);
	void lightSetup(unsigned int shaderIndex, const PointLight &light);
	void lightSetup(unsigned int shaderIndex, const SpotLight &light);

	void cameraSetup();

	GLFWwindow* _window;

	std::shared_ptr<UserInput::glfwKeyboardScanner> _userInput;
	std::shared_ptr<Camera> _camera;

	std::vector<std::shared_ptr<Shader>> _shaderPrograms;
	unsigned int currentShaderIndex = 0;
};
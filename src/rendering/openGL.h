#pragma once

// STD library includes
#include <iostream>

// GLFW include
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
#include "rendering/framebuffer/Framebuffer_Manager.h"
#include "user_input/glfwUserInputScanner.h"
#include "util/Arithmetic.h"

class openGL : public GraphicalEngine
{
public:
	openGL();
	~openGL();

	int initialize();

	void renderFrame(std::shared_ptr<Scene> scene) override;

	// Window
	void resizeWindow(GLFWwindow *window, int width, int height);
	GLFWwindow* getWindowPtr();

	// Features
	void enable(GLuint feature);
	void disable(GLuint feature);

	// Models
	void initializeMesh(Mesh& mesh);
	void initializeTexture(Texture& texture);
	void bindTextures(Mesh &mesh);
	void renderModel(ModelObject &model);

	// Shaders
	unsigned int getShaderProgramID(unsigned int shaderIndex) const;
	void useShader(unsigned int shaderIndex);

	// Framebuffers
	std::shared_ptr<FBO> addFBO(E_AttachmentFormat format, int width, int height);
	void bindFBO(unsigned int fboIndex);
	void unbindFBO();
	unsigned int getFBOIndex(std::shared_ptr<FBO> fbo) const;
	bool isFrameBufferComplete(std::shared_ptr<FBO>) const;
	FBOManager& getFBOManager();


private:
	void allLightsSetup(const LightContents &lights);
	void lightSetup(unsigned int shaderIndex, const DirectionalLight &light);
	void lightSetup(unsigned int shaderIndex, const PointLight &light);
	void lightSetup(unsigned int shaderIndex, const SpotLight &light);

	void cameraSetup(std::shared_ptr<Scene> scene);

	// GLFW window
	GLFWwindow* _window;

	// Scene
	std::shared_ptr<UserInput::glfwKeyboardScanner> _userInput;

	// Framebuffer
	std::unique_ptr<FBOManager> _frameBuffers;

	// Shaders
	std::vector<std::shared_ptr<Shader>> _shaderPrograms;
	unsigned int currentShaderIndex = 0;

};
#pragma once

// STD library includes
#include <iostream>

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// First party includes
#include "rendering/GraphicalEngine.hpp"
#include "rendering/shader/ShaderLibrary.hpp"
#include "resources/Texture.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "user_input/glfwUserInputScanner.hpp"
#include "util/Arithmetic.hpp"

class openGL : public GraphicalEngine
{
public:
	openGL();
	~openGL();

	int initialize(GLFWwindow* window);

	void renderFrame(std::shared_ptr<Scene> scene) override;

	// Window
	void resizeWindow(GLFWwindow *window, int width, int height);
	GLFWwindow* getWindowPtr();

	// Features
	void enable(GLuint feature);
	void disable(GLuint feature);

	// Models
	void initializeMesh(std::shared_ptr<Mesh>& mesh) override;
	void initializeTexture(Texture& texture) override;
	void bindTextures(std::shared_ptr<Mesh> mesh);
	void renderModel(ModelObject &model);
	void renderInstancedMeshes();

	// Shaders
	unsigned int getShaderProgramID(unsigned int shaderIndex);

	// Framebuffers
	std::shared_ptr<FBO> addFBO(E_AttachmentFormat format, int width, int height);
	void bindFBO(unsigned int fboIndex);
	void unbindFBO();
	unsigned int getFBOIndex(std::shared_ptr<FBO> fbo) const;
	bool isFrameBufferComplete(std::shared_ptr<FBO>) const;
	FBOManager& getFBOManager();

	// Cubemaps
	void initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures) override;
	void renderSkybox(Skybox &skybox);

	// Instancing
	void initializeInstanceManager(std::shared_ptr<Scene> scene);

private:

	void cameraSetup(std::shared_ptr<Scene> scene);

	// GLFW window
	GLFWwindow* _window;
	int _width, _height;

	// Framebuffer
	std::shared_ptr<FBOManager> _frameBuffers;

	// Shaders
	std::shared_ptr<ShaderLibrary> _shaderPrograms;

	// Lights
	std::shared_ptr<LightLibrary> _lightLibrary;

	//Instancing
	std::shared_ptr<InstancingManager> _instancingManager;
};
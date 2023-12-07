// First-party includes
#include "rendering/GLFW_Wrapper.hpp"
#include "rendering/openGLContext.hpp"
#include "rendering/openGL.hpp"
#include "scene/SceneObjectFactory.hpp"
#include "util/SceneSetup.h"
#include "Util/Logger.hpp"

// Third-party includes
#include <memory>

void update(openGL& graphicalEngine, std::vector<std::shared_ptr<Scene>> scenes, std::shared_ptr<UserInput::glfwKeyboardScanner>& userInput)
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    graphicalEngine.initializeInstanceManager(scenes[0]);

    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;
    float deltaTime = 0.0f;
    std::string windowTitle;
    glfwSwapInterval(1);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(graphicalEngine.getWindowPtr()))
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        deltaTime = newTime - gameTime - startTime;
        gameTime = newTime - startTime;

        openGLContext::updateFPSCounter(deltaTime);

        graphicalEngine.renderFrame(scenes[0]);

        // Check and log errors
        //Logger::Instance().logGLErrors("Main Loop:");        
       
        /* Swap front and back buffers */
        glfwSwapBuffers(graphicalEngine.getWindowPtr());
        /* Poll for and process events */
        glfwPollEvents();

        userInput->tickCallback();
    }
}

int main(void)
{
    GLFWwindow* window = InitializeOpenGLContext();

    if(window == nullptr)
    {
        return -1;
    }

    // Scene initialization
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::shared_ptr<MeshLibrary> meshLibrary = std::make_shared<MeshLibrary>();

    openGL graphicalEngine;
    graphicalEngine.initialize(window);
    graphicalEngine.bindScene(scene);
    graphicalEngine.bindMeshLibrary(meshLibrary);

    // Init object factory
    SceneObjectFactory factory(scene.get(), &graphicalEngine, meshLibrary.get());

    // Scene setup
    std::vector<std::shared_ptr<Scene>> scenes;
    scenes.push_back(scene);
    scene02_Setup(factory);

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());
    userInput->bindToScene(scene);

    // Begin main loop
    update(graphicalEngine, scenes, userInput);

    glfwTerminate();
    return 0;
}
// First-party includes
#include "rendering/GLFW_Wrapper.hpp"
#include "rendering/openGLContext.hpp"
#include "rendering/openGL.hpp"
#include "scene/SceneObjectFactory.hpp"
#include "util/SceneSetup.h"

// Third-party includes
#include <memory>

void update(openGL& graphicalEngine, std::vector<std::shared_ptr<Scene>> scenes, std::shared_ptr<UserInput::glfwKeyboardScanner>& userInput)
{

    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;
    float deltaTime = 0.0f;
    std::string windowTitle;

    while (!glfwWindowShouldClose(graphicalEngine.getWindowPtr()))
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        deltaTime = newTime - gameTime - startTime;
        gameTime = newTime - startTime;

        openGLContext::updateFPSCounter(deltaTime);

        graphicalEngine.renderFrame(scenes[0]);

        // Check and log errors
       
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

    openGL graphicalEngine;
    graphicalEngine.initialize(window, E_RenderStrategy::PBSShading);
    graphicalEngine.bindScene(scene);

    // Init object factory
    SceneObjectFactory factory(scene.get(), &graphicalEngine);

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
// OPENGL STUFF

// First-party includes
#include "rendering/GLFW_Wrapper.h"
#include "rendering/openGL.h"
#include "util/tickHandlerManager.h"
#include "util/tickHandler.h"

#include <memory>



void update(openGL& graphicalEngine, std::shared_ptr<UserInput::glfwKeyboardScanner>& userInput)
{
    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(graphicalEngine.getWindowPtr()))
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        gameTime = newTime - startTime;

        /* Render here */
        graphicalEngine.renderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(graphicalEngine.getWindowPtr());

        /* Poll for and process events */
        glfwPollEvents();

        userInput->tickCallback();
    }
}

int main(void)
{
    
    openGL graphicalEngine;
    graphicalEngine.initialize();

    // Scene initialization
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->initialize();
    graphicalEngine.bindScene(scene);

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());


    update(graphicalEngine, userInput);

    glfwTerminate();


    return 0;
}
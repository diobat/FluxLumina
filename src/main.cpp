// OPENGL STUFF

// First-party includes
#include "rendering/GLFW_Wrapper.h"
#include "rendering/openGL.h"
#include "scene/SceneObjectFactory.h"

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
    // Scene initialization
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    openGL graphicalEngine;
    graphicalEngine.initialize();
    graphicalEngine.bindScene(scene);

    // Init object factory
    SceneObjectFactory factory(&(*scene), &graphicalEngine);

    factory.create_Camera();

    ModelObject &statue = factory.create_Model("res/models/alliance.obj", "res/models/alliance.png");
    statue.setPosition({0.0f, 0.0f, -5.0f});




    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());


    update(graphicalEngine, userInput);

    glfwTerminate();


    return 0;
}
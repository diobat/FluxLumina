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

    // ModelObject &statue = factory.create_Model("res/models/alliance.obj", "res/models/alliance.png");
    // statue.setPosition({0.0f, 0.0f, -2.5f});
    // statue.rotate(200.0f, 180.0f, 0.0f);

    // ModelObject &statue2 = factory.create_Model("res/models/alliance.obj", "res/models/alliance.png");
    // statue2.setPosition({0.0f, 0.0f, 2.5f});
    // statue2.rotate(0.0f, 45.0f, 45.0f);

    ModelObject &ground = factory.create_Model("res/models/ground.obj");
    ground.setPosition({0.0f, -0.5f, 0.0f});

    ModelObject& mothership = factory.create_Model("res/models/Mothership/Mothership.obj");
    mothership.setPosition({0.0f, 10.0f, 7.5f});
    mothership.setScale(0.001f);
    //mothership.rotate(0.0f, 180.0f, 90.0f);

    ModelObject& backpack = factory.create_Model("res/models/backpack/backpack.obj", true);
    backpack.setPosition({0.0f, 3.0f, -7.5f});

    auto light = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light->setPosition({5.0f, 5.0f, -5.0f});
    light->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());
    update(graphicalEngine, userInput);

    glfwTerminate();
    return 0;
}
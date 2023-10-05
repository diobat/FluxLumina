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

    ModelObject &ground = factory.create_Model("res/models/ground.obj");
    ground.setPosition({0.0f, -0.5f, 0.0f});

    ModelObject& mothership = factory.create_Model("res/models/Mothership/Mothership.obj");
    mothership.setPosition({0.0f, 10.0f, 7.5f});
    mothership.setScale(0.001f);
    //mothership.rotate(0.0f, 180.0f, 90.0f);

    ModelObject& backpack = factory.create_Model("res/models/backpack/backpack.obj", 0, true);
    backpack.setPosition({0.0f, 3.0f, -7.5f});

    auto& statue = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 1);
    statue.setPosition({0.0f, 0.0f, 5.0f});
    //statue.rotate(-90.0f, 0.0f, 180.0f);
    statue.rotate(-90.0f, 0.0f, 0.0f);
    statue.setScale(0.01f);

    auto &statue2 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue2.setPosition({0.0f, 0.0f, -5.0f});
    //statue2.rotate(-90.0f, 0.0f, 0.0f);
    statue2.rotate(-90.0f, 0.0f, 0.0f);
    statue2.setScale(0.01f);

    auto &statue3 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 1);
    statue3.setPosition({-5.0f, 0.0f, 0.0f});
    //statue3.rotate(-90.0f, 0.0f, 90.0f);
    statue3.rotate(-90.0f, 0.0f, 0.0f);
    statue3.setScale(0.01f);

    auto &statue4 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue4.setPosition({5.0f, 0.0f, 0.0f});
    //statue4.rotate(-90.0f, 0.0f, -90.0f);
    statue4.rotate(-90.0f, 0.0f, 0.0f);
    statue4.setScale(0.01f);

    auto light = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light->setPosition({5.0f, 5.0f, -5.0f});
    light->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light2 = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light2->setPosition({-15.0f, 5.0f, -5.0f});
    light2->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light3 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light4 = std::dynamic_pointer_cast<SpotLight>(light3);
    light4->setPosition({0.0f, 10.0f, 0.0f});
    // light4->setDirection({0.0f, -1.0f, -1.5f});
    light4->pointAt({0.0f, 0.0f, 0.0f});

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());
    update(graphicalEngine, userInput);

    glfwTerminate();
    return 0;
}
// Purpose: Entry point of the program.

// First-party includes
#include "rendering/GLFW_Wrapper.h"
#include "rendering/openGLContext.h"
#include "rendering/openGL.h"
#include "scene/SceneObjectFactory.h"

// Third-party includes
#include <memory>


void update(openGL& graphicalEngine, std::vector<std::shared_ptr<Scene>> scenes, std::shared_ptr<UserInput::glfwKeyboardScanner>& userInput)
{
    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(graphicalEngine.getWindowPtr()))
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        gameTime = newTime - startTime;

        graphicalEngine.renderFrame(scenes[0]);

        /* Swap front and back buffers */
        glfwSwapBuffers(graphicalEngine.getWindowPtr());

        /* Poll for and process events */
        glfwPollEvents();

        userInput->tickCallback();
    }
}

int main(void)
{

    // UniformBuffer UBO;

    // std::tuple<float, glm::vec3, glm::mat4, float[3], bool, int> example;
    // std::tuple<glm::mat4, glm::mat4, glm::mat4> example2;

    // std::cout << "Total buffer size is :" << UBO.calculateUniformBufferSize(example) << std:: endl;


    /// Actual program begins here

    GLFWwindow* window = InitializeOpenGLContext();

    if(window == nullptr)
    {
        return -1;
    }

    // Scene initialization
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    openGL graphicalEngine;
    graphicalEngine.initialize(window);
    graphicalEngine.bindScene(scene);

    // Init object factory
    SceneObjectFactory factory(&(*scene), &graphicalEngine);

    // Camera setup
    factory.create_Camera();

    // Skybox setup
    auto skybox = factory.create_Skybox(
        {"res/models/skybox/right.jpg",
         "res/models/skybox/left.jpg",
         "res/models/skybox/top.jpg",
         "res/models/skybox/bottom.jpg",
         "res/models/skybox/front.jpg",
         "res/models/skybox/back.jpg"});

    // Scene objects 

    auto &window1 = factory.create_Model("res/models/window/window.obj", 2);
    window1.setPosition({-10.0f, 10.0f, 10.0f});

    auto &window2 = factory.create_Model("res/models/window/window.obj", 2);
    window2.setPosition({-10.0f, 10.0f, 8.0f});

    auto &window3 = factory.create_Model("res/models/window/window.obj", 2);
    window3.setPosition({-10.0f, 10.0f, 6.0f});

    ModelObject &ground = factory.create_Model("res/models/ground.obj", 0);
    ground.setPosition({0.0f, -0.5f, 0.0f});

    ModelObject& mothership = factory.create_Model("res/models/Mothership/Mothership.obj");
    mothership.setPosition({0.0f, 10.0f, 7.5f});
    mothership.setScale(0.001f);

    ModelObject& backpack = factory.create_Model("res/models/backpack/backpack.obj", 0, true);
    backpack.setPosition({0.0f, 3.0f, -7.5f});

    auto& statue = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 1);
    statue.setPosition({0.0f, 0.0f, 5.0f});
    statue.rotate(-90.0f, 0.0f, 0.0f);
    statue.setScale(0.01f);

    auto &statue2 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue2.setPosition({0.0f, 0.0f, -5.0f});
    statue2.rotate(-90.0f, 0.0f, 0.0f);
    statue2.setScale(0.01f);

    auto &statue3 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 1);
    statue3.setPosition({-5.0f, 0.0f, 0.0f});
    statue3.rotate(-90.0f, 0.0f, 0.0f);
    statue3.setScale(0.01f);

    auto &statue4 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue4.setPosition({5.0f, 0.0f, 0.0f});
    statue4.rotate(-90.0f, 0.0f, 0.0f);
    statue4.setScale(0.01f);

    auto& grass = factory.create_Model("res/models/grassSquare/grassSquare.obj", 2);
    grass.setPosition({10.0f, 1.0f, 10.0f});

    auto& cube = factory.create_Model("res/models/cube/cube.obj", 5);
    std::vector<Texture> cubeTextures = {scene->getSkybox().getCubemap()->getTexture()};
    cube.getModel()->meshes[0].attachTexture(cubeTextures);
    cube.setPosition({10.0f, 10.0f, 10.0f});

    // Lights   

    auto light = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light->setPosition({5.0f, 5.0f, -5.0f});
    light->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light2 = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light2->setPosition({-15.0f, 5.0f, -5.0f});
    light2->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light3 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light4 = std::dynamic_pointer_cast<SpotLight>(light3);
    light4->setPosition({0.0f, 10.0f, 0.0f});
    light4->pointAt({0.0f, 0.0f, 0.0f});

    std::vector<std::shared_ptr<Scene>> scenes;
    scenes.push_back(scene);

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());
    userInput->bindToScene(scene);
    update(graphicalEngine, scenes, userInput);

    glfwTerminate();
    return 0;
}
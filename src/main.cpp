// First-party includes
#include "rendering/GLFW_Wrapper.hpp"
#include "rendering/openGLContext.hpp"
#include "rendering/openGL.hpp"
#include "scene/SceneObjectFactory.hpp"
#include "util/Logger.hpp"

// Third-party includes
#include <memory>


void update(openGL& graphicalEngine, std::vector<std::shared_ptr<Scene>> scenes, std::shared_ptr<UserInput::glfwKeyboardScanner>& userInput)
{
    graphicalEngine.initializeInstanceManager(scenes[0]);

    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;
    float deltaTime = 0.0f;
    std::string windowTitle;
    glfwSwapInterval(0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(graphicalEngine.getWindowPtr()))
    {
        Logger::Instance().tic();
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        deltaTime = newTime - gameTime - startTime;
        gameTime = newTime - startTime;

        windowTitle = "Espigão Simulator 3000 - " + std::to_string(1.0f / deltaTime) + " FPS";
        glfwSetWindowTitle(graphicalEngine.getWindowPtr(), windowTitle.c_str());

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

    /// Actual program begins here

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

    // ModelObject& mothership = factory.create_Model("res/models/Mothership/Mothership.obj", 0);
    // mothership.setPosition({0.0f, 10.0f, 7.5f});
    // mothership.setScale(0.001f);

    ModelObject& backpack = factory.create_Model("res/models/backpack/backpack.obj", 0, true);
    backpack.setPosition({10.0f, 3.0f, -7.5f});


    int width = 700;
    int height = 700;

    // for(int i(0); i < width * height; ++i)
    // {
    //     auto& cubeN = factory.create_Model("res/models/cube/cube.obj", 0);
    //     cubeN.setPosition({((i % width) * 2.0f) * 10.0f, -15.0f, ((i / width) * 2.0f) * 10.0f});
    //     //cubeN.rotate(45.0f, 45.0f, 45.0f);
    //     cubeN.setScale(5.0f);
    // }

    auto& statue = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue.setPosition({0.0f, 0.0f, 5.0f});
    statue.rotate(-90.0f, 0.0f, 0.0f);
    statue.setScale(0.01f);

    auto &statue2 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue2.setPosition({0.0f, 0.0f, -5.0f});
    statue2.rotate(-90.0f, 0.0f, 0.0f);
    statue2.setScale(0.01f);

    auto &statue3 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue3.setPosition({-5.0f, 0.0f, 0.0f});
    statue3.rotate(-90.0f, 0.0f, 0.0f);
    statue3.setScale(0.01f);

    auto &statue4 = factory.create_Model("res/models/Statue/12330_Statue_v1_L2.obj", 0);
    statue4.setPosition({5.0f, 0.0f, 0.0f});
    statue4.rotate(-90.0f, 0.0f, 0.0f);
    statue4.setScale(0.01f);

    auto& grass = factory.create_Model("res/models/grassSquare/grassSquare.obj", 2);
    grass.setPosition({10.0f, 1.0f, 10.0f});

    auto& cube = factory.create_Model("res/models/cube/cubeBlank.obj", 5);
    std::vector<Texture> cubeTextures = {scene->getSkybox().getCubemap()->getTexture()};
    cube.getModel()->meshes[0]->attachTexture(cubeTextures);
    cube.setPosition({10.0f, 10.0f, 10.0f});
    
    // Lights
    auto light_A = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_A->setPosition({15.0f, 5.0f, 15.0f});
    light_A->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light_B = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_B->setPosition({-15.0f, 5.0f, 15.0f});
    light_B->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light_C = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_C->setPosition({15.0f, 5.0f, -15.0f});
    light_C->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light_D = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_D->setPosition({-15.0f, 5.0f, -15.0f});
    light_D->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light3 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light4 = std::dynamic_pointer_cast<SpotLight>(light3);
    light4->setCutoff(18.5f);
    light4->setPosition({0.0f, 10.0f, 0.0f});
    light4->pointAt(statue.getPosition());

    auto light5 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light6 = std::dynamic_pointer_cast<SpotLight>(light5);
    light6->setCutoff(18.5f);
    light6->setPosition({0.0f, 10.0f, 0.0f});
    light6->pointAt(statue2.getPosition());

    auto light7 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light8 = std::dynamic_pointer_cast<SpotLight>(light7);
    light8->setCutoff(18.5f);
    light8->setPosition({0.0f, 10.0f, 0.0f});
    light8->pointAt(statue3.getPosition());

    auto light9 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light10 = std::dynamic_pointer_cast<SpotLight>(light9);
    light10->setCutoff(18.5f);
    light10->setPosition({0.0f, 10.0f, 0.0f});
    light10->pointAt(statue4.getPosition());

    //light4->pointAt({1.0f, 0.0f, 0.0f});

    // auto& cube5 = factory.create_Model("res/models/cube/cubeBlank.obj", 0);
    // cube5.setScale(3.0f);
    // cube5.setPosition(light4->getPosition());

    // Scene setup
    std::vector<std::shared_ptr<Scene>> scenes;
    scenes.push_back(scene);

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());
    userInput->bindToScene(scene);

    // Begin main loop
    update(graphicalEngine, scenes, userInput);

    glfwTerminate();
    return 0;
}
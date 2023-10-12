// OPENGL STUFF

// First-party includes
#include "rendering/GLFW_Wrapper.h"
#include "rendering/openGL.h"
#include "scene/SceneObjectFactory.h"

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

        //glBindFramebuffer(GL_FRAMEBUFFER, 1); 
        /* Render here */
        glEnable(GL_DEPTH_TEST);
        graphicalEngine.renderFrame(scenes[0]);

        // // second pass
        //glBindFramebuffer(GL_FRAMEBUFFER, 0); 

        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        // glUseProgram(graphicalEngine.getShaderProgramID(3));
        // glBindVertexArray(quadVAO);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, colorattachmentID);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        graphicalEngine.renderFrame(scenes[1]);

        ////////////////////////////////////////////////////////////
        // glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
        // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // int width, height;
        // glfwGetWindowSize(graphicalEngine.getWindowPtr(), &width, &height);

        // glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
        //                   GL_COLOR_BUFFER_BIT, GL_NEAREST);
        ////////////////////////////////////////////////////////////

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

    auto& FBOManager = graphicalEngine.getFBOManager();

    // Get window dimensions
    int width, height;
    glfwGetWindowSize(graphicalEngine.getWindowPtr(), &width, &height);
    auto FBO = graphicalEngine.addFBO(E_AttachmentFormat::RENDERBUFFER, width, height);
    FBO->addAttachment(E_AttachmentType::COLOR);
    FBO->addAttachment(E_AttachmentType::DEPTH);
    //FBO->addAttachment(E_AttachmentType::STENCIL);

    if(!graphicalEngine.isFrameBufferComplete(FBO))
    {
        std::cout << "Framebuffer is not complete" << std::endl;
        return -1;
    }

    FBOManager.bindSceneToFBO(scene, FBO);

    // Init object factory

    SceneObjectFactory factory(&(*scene), &graphicalEngine);

    factory.create_Camera();

    auto &window = factory.create_Model("res/models/window/window.obj", 2);
    window.setPosition({-10.0f, 10.0f, 10.0f});

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

    auto& grass = factory.create_Model("res/models/grassSquare/grassSquare.obj", 2);
    grass.setPosition({10.0f, 1.0f, 10.0f});

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


    // Scene 2

    std::shared_ptr<Scene> scene2 = std::make_shared<Scene>();
    factory.bindScene(&(*scene2));
    factory.create_Camera();

    ModelObject &quad = factory.create_Model("res/models/quad/quad.obj", 3);

    Texture tempText;
    tempText._id = graphicalEngine.getFBOManager().getFBO(0)->getColorAttachmentID(0);
    tempText._type = E_TexureType::DIFFUSE;
    tempText._colorChannels = GL_RGB;
    tempText._components = 3;
    tempText._useLinear = true;
    quad.getModel()->meshes[0].textures.push_back(tempText);

    std::vector<std::shared_ptr<Scene>> scenes;
    scenes.push_back(scene);
    scenes.push_back(scene2);

    // User Input handler
    std::shared_ptr<UserInput::glfwKeyboardScanner> userInput = std::make_shared<UserInput::glfwKeyboardScanner>(graphicalEngine.getWindowPtr());
    userInput->bindToScene(scene);
    update(graphicalEngine, scenes, userInput);

    glfwTerminate();
    return 0;
}
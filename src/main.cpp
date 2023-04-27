
#define ASSIMP_USE_HUNTER
// First-party includes
//#include "scene/Scene.h"
#include "rendering/openGL.h"


//
//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#define  GLM_FORCE_RADIANS
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include "rendering/Shader.h"
//#include "rendering/Texture.h"
//#include "rendering/Model.h"

GLFWwindow* window;
const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 768;

Model   * mesh    = nullptr;
Texture * texture = nullptr;
Camera cam;
Scene scene;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    cam.resizeCameraPlane(width, height);

}


int loadContent()
{

    return true;
}

void render(float time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.getAllModels().at(0).Draw();

}

void update()
{
    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;

    /* Loop until the user closes the window */
    //while (!glfwWindowShouldClose(window))
    while (true)
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        gameTime = newTime - startTime;

        /* Render here */
        render(gameTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main(void)
{

    openGL graphicalEngine;
    if (!graphicalEngine.initialize())
        return -1;


    if (!scene.initialize())
        return -1;


    if (!loadContent())
        return -1;

    update();

    glfwTerminate();


    delete texture;

    return 0;
}



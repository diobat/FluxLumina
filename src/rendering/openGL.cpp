#include "rendering/openGL.h"



void openGL::initialize()
{
    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);


    // Select shader program to use
    Shader shader("Basic.vert", "Basic.frag");
    _shaderPrograms.push_back(shader);
    _shaderPrograms[0].apply();


    _shaderPrograms[0].setUniformMatrix4fv("MVP", _camera->recalculateMVP());


}

void openGL::renderFrame()
{

    //_camera

}

void openGL::resizeWindow(GLFWwindow* window, int width, int height)
{
    //glViewport(0, 0, width, height);

    //if (shader != nullptr)
    //{
    //    shader->setUniformMatrix4fv("viewProj", _camera->getMVP());
    //}
}
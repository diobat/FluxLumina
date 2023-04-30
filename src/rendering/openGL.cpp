#include "rendering/openGL.h"

openGL::openGL(Scene& scene)
    : _scene(scene),
    _camera(std::make_shared<Camera>())
{

}

int openGL::initialize()
{

    //int WINDOW_WIDTH = 1024;
    //int WINDOW_HEIGHT = 768;

    ///* Initialize the library */
    //if (!glfwInit())
    //    return 0;

    ///* Create a windowed mode window and its OpenGL context */
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello Modern GL!", nullptr, nullptr);

    //if (!_window)
    //{
    //    glfwTerminate();
    //    return 0;
    //}

    ///* Make the window's context current */
    //glfwMakeContextCurrent(_window);
    //



    /////* Initialize glad */
    ////if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    ////{
    ////    std::cout << "Failed to initialize GLAD" << std::endl;
    ////    return 0;
    ////}




    ///* Set the viewport */
    //glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);
    //glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    //glEnable(GL_DEPTH_TEST);

    //// Accept fragment if it closer to the camera than the former one
    //glDepthFunc(GL_LESS);


    // Select shader program to use
    Shader shader("Basic.vert", "Basic.frag");
    _shaderPrograms.push_back(shader);
    _shaderPrograms[0].apply();

    Texture newTexture;
    newTexture.load("res/models/alliance.png");
    _textures.push_back(newTexture);


    return 1;
}

void openGL::renderFrame()
{
    // Shader
    _shaderPrograms[0].apply();


    // Camera
    _shaderPrograms[0].setUniformMatrix4fv("MVP", _camera->recalculateMVP());
    std::cout << "a" << std::endl;

    // Textures & Models
    _textures[0].bind();
    for (auto model : _scene.getAllModels())
    {
        model.Draw();
    }



}

//void openGL::resizeWindow(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//
//    if (_camera != nullptr)
//    {
//        _camera->resizeCameraPlane(width, height);
//    }
//}
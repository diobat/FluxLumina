#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.h"

// STD library includes
#include <iostream>


namespace openGLContext
{
    GLFWwindow* window;
    int WINDOW_WIDTH = 2048;
    int WINDOW_HEIGHT = 1536;

    int CreateOpenGLWindow();
    int InitializeOpenGLContext();
}

GLFWwindow* CreateOpenGLWindow()
{

    using namespace openGLContext;
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Espigão Simulator 3000", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        return 0;
    }

    return window;
}

GLFWwindow *InitializeOpenGLContext()
{
    /* Initialize the library */
    if (!glfwInit())
        return 0;

    GLFWwindow *window = CreateOpenGLWindow();

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    return window;
}
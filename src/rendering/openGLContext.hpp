#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

// First-party includes
#include "helpers/RootDir.hpp"

// STL library includes
#include <array>
#include <string>
#include <cmath>
#include <iostream>

// Third-party includes
#include <stb_image.h>

namespace openGLContext
{
    GLFWwindow* window;
    int WINDOW_WIDTH = 2048;
    int WINDOW_HEIGHT = 1536;
    std::string windowTitle = "FluxLumina";

    int CreateOpenGLWindow();
    int InitializeOpenGLContext();

    std::array<float, 100> deltaFrameTimes = {0.0f};
    unsigned int deltaFrameTimeIndex = 0;

    void updateFPSCounter(float delta)
    {
        deltaFrameTimes[deltaFrameTimeIndex] = delta;
        deltaFrameTimeIndex = (deltaFrameTimeIndex + 1) % deltaFrameTimes.size();

        double averageDelta = 0.0f;
        for (const auto& deltaFrameTime : deltaFrameTimes)
        {
            averageDelta += deltaFrameTime;
        }
        averageDelta /= deltaFrameTimes.size();
        averageDelta = std::round(1.0f / averageDelta);
        std::string fps = std::to_string(averageDelta); // Convert to time per frame
        fps.erase(fps.find_last_not_of('0'), std::string::npos); // Remove trailing zeros
        std::string fullWindowTitle = windowTitle + " - " + fps + " FPS";
        glfwSetWindowTitle(window, fullWindowTitle.c_str());   
    }
}





GLFWwindow* CreateOpenGLWindow()
{

    using namespace openGLContext;
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "FluxLumina", nullptr, nullptr);

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

    // Window Icon
    GLFWimage images[1]; 
    std::string iconPath = PROJECT_ROOT_DIR + std::string("/res/icon.png");
    images[0].pixels = stbi_load(iconPath.c_str(), &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images); 
    stbi_image_free(images[0].pixels);

    return window;
}
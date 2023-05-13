// OPENGL STUFF

// First-party includes
#include "rendering/GLFW_Wrapper.h"
#include "rendering/openGL.h"


void update(openGL& graphics)
{
    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(graphics.getWindowPtr()))
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        gameTime = newTime - startTime;

        /* Render here */
        graphics.renderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(graphics.getWindowPtr());

        /* Poll for and process events */
        glfwPollEvents();
    }
}

int main(void)
{
    //if (!init())
    //    return -1;

    openGL graphics;
    graphics.initialize();

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->initialize();
    graphics.bindScene(scene);

    //if (!loadContent())
    //    return -1;

    update(graphics);

    glfwTerminate();


    return 0;
}
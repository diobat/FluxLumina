// First-party includes
#include "rendering/GLFW_Wrapper.hpp"

#include "rendering/FluxLumina.hpp"
#include "scene/SceneObjectFactory.hpp"
#include "util/SceneSetup.h"

// Third-party includes
#include <memory>

int main(void)
{
    // Scene initialization
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    FluxLumina graphicalEngine(scene, E_RenderStrategy::ForwardShading);

    // Init object factory
    SceneObjectFactory factory(scene.get(), &graphicalEngine);

    // Scene setup
    std::vector<std::shared_ptr<Scene>> scenes;
    scenes.push_back(scene);
    scene01_Setup(factory);

    // Begin main loop
    graphicalEngine.update();

    glfwTerminate();
    return 0;
}
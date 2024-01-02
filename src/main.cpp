// First-party includes
#include "rendering/FluxLumina.hpp"
#include "util/SceneSetup.h"
// Third-party includes
#include <memory>

int main(void)
{
    FluxLumina graphicalEngine(E_RenderStrategy::ForwardShading);
    scene01_Setup(graphicalEngine);
    // Begin main loop
    graphicalEngine.update();
    return 0;
}
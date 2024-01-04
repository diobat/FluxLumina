// First-party includes
#include "FluxLumina.hpp"
#include "util/SceneSetup.h"

int main(void)
{
    FluxLumina graphicalEngine(E_RenderStrategy::PBSShading);
    scene02_Setup(graphicalEngine);
    // Begin main loop
    graphicalEngine.update();
    return 0;
}
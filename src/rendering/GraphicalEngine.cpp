#include "GraphicalEngine.hpp"

#include "rendering/MeshLibrary.hpp"
#include "rendering/strategy/StrategyChain.hpp"
#include "rendering/shader/ShaderLibrary.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"
#include "rendering/libraries/TextureLibrary.hpp"
#include "rendering/Settings.hpp"
#include "user_input/glfwUserInputScanner.hpp"

GraphicalEngine::GraphicalEngine()
{
    ;
}

void GraphicalEngine::bindScene(std::shared_ptr<Scene> scene)
{
    _scenes.push_back(scene);
    _frameBuffers->parseNewScene(scene);
}

void GraphicalEngine::unbindScene(std::shared_ptr<Scene> sceneToUnbind)
{
    _scenes.erase(std::remove_if(_scenes.begin(), _scenes.end(), 
    [sceneToUnbind](std::shared_ptr<Scene> scene) { 
        return scene == sceneToUnbind; 
        }),
        _scenes.end());
}

std::shared_ptr<Scene> GraphicalEngine::getScene(unsigned int sceneIndex) const
{
    if(sceneIndex >= _scenes.size())
    {
        return nullptr;
    }

    return _scenes[sceneIndex];
}



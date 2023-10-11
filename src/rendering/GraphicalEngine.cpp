#include "GraphicalEngine.h"

GraphicalEngine::GraphicalEngine()
{
    ;
}

void GraphicalEngine::bindScene(std::shared_ptr<Scene> scene)
{
    // _scenes.push_back(scene);
    _scene = scene;
}

void GraphicalEngine::unbindScene(std::shared_ptr<Scene> sceneToUnbind)
{
    // _scenes.erase(std::remove_if(_scenes.begin(), _scenes.end(), 
    // [sceneToUnbind](std::shared_ptr<Scene> scene) { 
    //     return scene == sceneToUnbind; 
    //     }),
    //     _scenes.end());
}
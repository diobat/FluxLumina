#include "GraphicalEngine.hpp"

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

void GraphicalEngine::bindMeshLibrary(std::shared_ptr<MeshLibrary> meshLibrary)
{
    _meshLibrary = meshLibrary;
}



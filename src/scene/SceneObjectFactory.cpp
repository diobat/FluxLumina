#include "scene/SceneObjectFactory.h"

void SceneObjectFactory::bindScene(Scene* scene)
{
    _boundScene = scene;
}

void SceneObjectFactory::bindEngine(GraphicalEngine *engine)
{
    _boundEngine = engine;
}

// void SceneObjectFactory::create(SCENE_OBJECT object)
// {
//     switch(object)
//     {
//         case MODEL:
//         create_Model();
//         break;

//         case CAMERA:
//         create_Camera();
//         break;

//         case LIGHTSOURCE:
//         create_LightSource();
//         break;
//     }

// }


void SceneObjectFactory::create_Model(const std::string& model, const std::string& texture)
{

}

void SceneObjectFactory::create_LightSource()
{

}

void SceneObjectFactory::create_Camera()
{

}
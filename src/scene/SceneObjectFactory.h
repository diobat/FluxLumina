#pragma once

// First-party includes
#include "scene/Scene.h"
#include "scene/SceneObject.h"
#include "rendering/GraphicalEngine.h"
#include "scene/ModelObject.h"
#include "scene/Camera.h"
#include "scene/LightSource.h"




/*
    This is the class that manages the proper creation and release of resources for rendered objects
    It manages safe coordination between the scene and rendering engine while allowing the first to not be aware of the latter
*/

class SceneObjectFactory
{
public:

    void bindScene(Scene* scene);
    void bindEngine(GraphicalEngine* engine);
    //void create(SCENE_OBJECT object);

    void create_Model(const std::string& model, const std::string& texture);
    void create_LightSource();
    void create_Camera();

private:

    Scene* _boundScene;
    GraphicalEngine* _boundEngine;
};

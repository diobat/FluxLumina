#pragma once

// First-party includes
#include "scene/sceneObject.h"
#include "rendering/texture.h"
#include "rendering/model.h"
#include "util/Listener.h"

#include <string>

class ModelObject : public SceneObject, public std::enable_shared_from_this<ModelObject>
{
public:
    ModelObject();
    virtual ~ModelObject();	

    void setModel(const std::shared_ptr<Model> &model);
    std::shared_ptr<Model> getModel();

    void setShaderIndex(unsigned int shaderIndex);
    unsigned int getShaderIndex() const;

    Broadcaster& getBroadcaster();

private:
    std::shared_ptr<Model> _model;
    unsigned int _shaderIndex;

    Broadcaster _broadcaster;
};

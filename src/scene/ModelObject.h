#pragma once

// First-party includes
#include "SceneObject.h"

#include "rendering/texture.h"
#include "rendering/model.h"

#include <string>

class ModelObject : public SceneObject
{
public:
    ModelObject();
    virtual ~ModelObject();	

    void setModel(const std::shared_ptr<Model> &model);
    std::shared_ptr<Model> getModel();

private:
    std::shared_ptr<Model> _model;
};

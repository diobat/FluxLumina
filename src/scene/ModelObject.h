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

    void setModel(const std::shared_ptr<Model> &model);
    std::shared_ptr<Model> getModel();
    void setTexture(const std::shared_ptr<Texture> &texture);
    std::shared_ptr<Texture> getTexture();

private:
    std::shared_ptr<Model> _model;
    std::shared_ptr<Texture> _texture;
};

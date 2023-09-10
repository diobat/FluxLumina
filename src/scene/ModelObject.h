#pragma once

// First-party includes
#include "SceneObject.h"

#include "rendering/texture.h"
#include "rendering/model.h"

#include <string>

class ModelObject : public SceneObject
{
public:

    ModelObject(const char *modelPath, const char * texturePath);
    ModelObject(std::string modelPath, std::string texturePath);

    void setModel(const std::string& modelPath);
    void setTexture(const std::string& texturePath);

    std::unique_ptr<Model>& getModel();

private:
    std::unique_ptr<Model> _model;
    std::unique_ptr<Texture> _texture;
};

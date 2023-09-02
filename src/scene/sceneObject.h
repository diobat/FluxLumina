#pragma once

#include "rendering/texture.h"
#include "rendering/model.h"

#include <string>

class SceneObject
{
public:

    void setModel(const std::string& modelPath);
    void setTexture(const std::string& texturePath);

private:

    std::unique_ptr<Model> _model;
    std::unique_ptr<Texture> _texture;
};

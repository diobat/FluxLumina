#include "ModelObject.h"

ModelObject::ModelObject(const char *modelPath, const char *texturePath)
{
    std::string model = modelPath;
    std::string texture = texturePath;
    ModelObject(model, texture);
}

ModelObject::ModelObject(std::string modelPath, std::string texturePath)
{
    _model = std::make_unique<Model>(modelPath);

    Texture newTexture;
    newTexture.load(texturePath);
    _texture = std::make_unique<Texture>(std::move(newTexture));
}



void ModelObject::setModel(const std::string &modelPath)
{
    _model = std::make_unique<Model>(modelPath);
}

void ModelObject::setTexture(const std::string &texturePath)
{
    Texture newTexture;
    newTexture.load(texturePath);
    _texture = std::make_unique<Texture>(newTexture);
}

std::unique_ptr<Model>& ModelObject::getModel()
{
    return _model;
}
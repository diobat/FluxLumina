#include "sceneObject.h"


void SceneObject::setModel(const std::string& modelPath)
{
    _model = std::make_unique<Model>(modelPath);
}

void SceneObject::setTexture(const std::string& texturePath)
{
    Texture newTexture;
    newTexture.load(texturePath);
    _texture = std::make_unique<Texture>(newTexture);
}

#include "ModelObject.h"


ModelObject::ModelObject()  :
    _model(std::make_shared<Model>())
{

}

void ModelObject::setModel(const std::shared_ptr<Model> &model)
{
    _model = model;
}

void ModelObject::setTexture(const std::shared_ptr<Texture> &texture)
{
    _texture = texture;
}

std::shared_ptr<Model> ModelObject::getModel()
{
    return _model;
}

std::shared_ptr<Texture> ModelObject::getTexture()
{
    return _texture;
}

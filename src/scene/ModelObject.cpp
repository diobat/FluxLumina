#include "ModelObject.h"


ModelObject::ModelObject()  :
    _model(std::make_shared<Model>())
{

}

void ModelObject::setModel(const std::shared_ptr<Model> &model)
{
    _model = model;
}

std::shared_ptr<Model> ModelObject::getModel()
{
    return _model;
}

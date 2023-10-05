#include "ModelObject.h"


ModelObject::ModelObject()  :
    _model(std::make_shared<Model>()),
    _shaderIndex(0)
{

}

ModelObject::~ModelObject()
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

void ModelObject::setShaderIndex(unsigned int shaderIndex)
{
    _shaderIndex = shaderIndex;
    _broadcaster.trigger(shared_from_this(), shaderIndex);
}

unsigned int ModelObject::getShaderIndex() const
{
    return _shaderIndex;
}

Broadcaster& ModelObject::getBroadcaster()
{
    return _broadcaster;
}
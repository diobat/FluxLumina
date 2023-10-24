#include "ModelObject.h"


ModelObject::ModelObject()  :
    _model(std::make_shared<Model>()),
    _shaderIndex(0),
    _modelMatrixProvider(_properties)
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

const glm::mat4& ModelObject::getModelMatrix()
{
    return _modelMatrixProvider.getModelMatrix();
}

Broadcaster& ModelObject::getBroadcaster()
{
    return _broadcaster;
}
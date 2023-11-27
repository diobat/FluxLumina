#include "ModelObject.hpp"

ModelObject::ModelObject()  :
    _model(std::make_shared<Model>()),
    _shaderIndex(0),
    _modelMatrixProvider(_properties),
    _id(boost::uuids::random_generator()())
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
}

void ModelObject::setShaderName(const std::string& shaderName)
{
    _assignedShader = shaderName;
}

const std::string& ModelObject::getShaderName() const
{
    return _assignedShader;
}

unsigned int ModelObject::getShaderIndex() const
{
    return _shaderIndex;
}

const glm::mat4& ModelObject::getModelMatrix()
{
    return _modelMatrixProvider.getModelMatrix();
}

const boost::uuids::uuid& ModelObject::uuid() const
{
    return _id;
}
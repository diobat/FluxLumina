#include "scene/ModelMatrixProvider.hpp"

ModelMatrixProvider::ModelMatrixProvider(const SceneObjectProperties& source)   :
    _sourceData(source),
    _modelMatrix(glm::mat4(1.0f))
{
    updateModelMatrix();
}

void ModelMatrixProvider::updateModelMatrix()
{
    glm::vec3 coordinates(_sourceData.coordinates[0], _sourceData.coordinates[1], _sourceData.coordinates[2]);

    // Model matrix
    _modelMatrix = glm::mat4(1.0f);
    // Apply translation to the model matrix
    _modelMatrix = glm::translate(_modelMatrix, coordinates);
    // Apply scale to the model matrix
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(_sourceData.scale));
    // Apply rotation to the model matrix
    _modelMatrix = _modelMatrix * glm::mat4_cast(_sourceData.rotation.getRotation());
}

const glm::mat4 &ModelMatrixProvider::getModelMatrix()
{
    if(_lastProperties.coordinates != _sourceData.coordinates ||
       _lastProperties.scale != _sourceData.scale ||
       _lastProperties.rotation.getRotation() != _sourceData.rotation.getRotation())
    {
        updateModelMatrix();
        _lastProperties = _sourceData;
    }
    return _modelMatrix;
}
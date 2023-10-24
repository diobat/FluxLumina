#pragma once

// GLM - Math is a gateway science
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// First party includes
#include "scene/SceneObjectProperties.h"

class ModelMatrixProvider
{
public:
    ModelMatrixProvider(const SceneObjectProperties &source);

    void updateModelMatrix();
    const glm::mat4& getModelMatrix();

private:
    const SceneObjectProperties& _sourceData;
    glm::mat4 _modelMatrix;

    SceneObjectProperties _lastProperties;
};


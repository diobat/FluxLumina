#pragma once

//	STD includes
#include <vector>
#include <memory>
#include <unordered_map>

//	First Party includes
#include "scene/ModelObject.h"

class ModelContents
{
public:
    void addModel(std::shared_ptr<ModelObject> modelToAdd);
    bool removeModel(std::shared_ptr<ModelObject> modelToRemove);
    bool moveModel(std::shared_ptr<ModelObject> modelToMove, unsigned int newShaderIndex);

    const std::vector<std::shared_ptr<ModelObject>> &getModels(unsigned int shaderIndex) const;
    void removeByShader(unsigned int shaderIndex);

    std::vector<unsigned int> getShaderIDs() const;
    unsigned int getDefaultShader() const;

    const std::unordered_map<unsigned int, std::vector<std::shared_ptr<ModelObject>>> &getAllModels() const;

private:
    std::unordered_map<unsigned int, std::vector<std::shared_ptr<ModelObject>>> _models;
    unsigned int defaultShaderIndex = 0;
};
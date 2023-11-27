#pragma once

//	STD includes
#include <vector>
#include <memory>
#include <unordered_map>

//	First Party includes
#include "scene/ModelObject.hpp"

class ModelContents
{
public:
    void addModel(std::shared_ptr<ModelObject> modelToAdd);
    bool removeModel(std::shared_ptr<ModelObject> modelToRemove);
    const std::vector<std::shared_ptr<ModelObject>> &getModels() const;
    std::vector<std::shared_ptr<ModelObject>> getModels(const std::string& shader) const;


private:
    std::vector<std::shared_ptr<ModelObject>> _models;

};
#include "scene/ModelContents.hpp"


void ModelContents::addModel(std::shared_ptr<ModelObject> model)
{
    _models.push_back(model);
}


bool ModelContents::removeModel(std::shared_ptr<ModelObject> modelToRemove)
{


    for(auto &model : _models)
    {
        if(model == modelToRemove)
        {
            _models.erase(std::remove(_models.begin(), _models.end(), modelToRemove), _models.end());
            return true;
        }
    }

    return false;
}

const std::vector<std::shared_ptr<ModelObject>> &ModelContents::getModels() const
{
    return _models;
}

std::vector<std::shared_ptr<ModelObject>> ModelContents::getModels(const std::string& shader) const
{
    std::vector<std::shared_ptr<ModelObject>> models;

    for(auto &model : _models)
    {
        if(model->getShaderName() == shader)
        {
            models.push_back(model);
        }
    }

    return models;
}
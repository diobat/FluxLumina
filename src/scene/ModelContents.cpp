#include "scene/ModelContents.hpp"


void ModelContents::addModel(std::shared_ptr<ModelObject> model)
{
    // If this is the first model in the scene, its shader index is the default shader index
    if(_models.empty())
        defaultShaderIndex = model.get()->getShaderIndex();

    _models[model->getShaderIndex()].push_back(model);

    // Register this object as listener to model
    model->getBroadcaster().addListener([&](std::shared_ptr<ModelObject> model, int newShaderIndex)
        {   
            moveModel(model, newShaderIndex); 
        }
        );
}


bool ModelContents::removeModel(std::shared_ptr<ModelObject> modelToRemove)
{
    // Remove model from list
    auto &modelList = _models[modelToRemove->getShaderIndex()];

    // Check if the modelToMove exists in the vector
    auto it = std::find(modelList.begin(), modelList.end(), modelToRemove);

    if(it == modelList.end())
        return false;
        //throw std::runtime_error("ModelContents::removeModel: modelToRemove not found in modelList");
    else
        modelList.erase(std::remove(modelList.begin(), modelList.end(), modelToRemove), modelList.end());

    return true;
}

bool ModelContents::moveModel(std::shared_ptr<ModelObject> modelToMove, unsigned int newShaderIndex)
{
    // Remove model from list
    auto &modelList = _models[modelToMove->getShaderIndex()];

    modelList.erase(std::remove_if(modelList.begin(), modelList.end(), [&modelToMove](const std::shared_ptr<ModelObject>& ptr) {
        return ptr == modelToMove;
        }),
        modelList.end());

    // Add model to new list
    _models[newShaderIndex].push_back(modelToMove);

    return true;
}

const std::vector<std::shared_ptr<ModelObject>> &ModelContents::getModels(unsigned int shaderIndex) const
{
    // Return empty vector if shaderIndex does not exist
    if (_models.count(shaderIndex) == 0)
    {
        static std::vector<std::shared_ptr<ModelObject>> emptyVector;
        return emptyVector;
    }
    return _models.at(shaderIndex);
}

void ModelContents::removeByShader(unsigned int shaderIndex)
{
    _models.erase(shaderIndex);
}

std::vector<unsigned int> ModelContents::getShaderIDs() const
{
    std::vector<unsigned int> shaderIndices;
    for (auto &shaderIndex : _models)
    {
        shaderIndices.push_back(shaderIndex.first);
    }
    return shaderIndices;
}

unsigned int ModelContents::getDefaultShader() const
{
    return defaultShaderIndex;
}

const std::unordered_map<unsigned int, std::vector<std::shared_ptr<ModelObject>>> &ModelContents::getAllModels() const
{
    return _models;
}

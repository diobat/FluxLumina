#include "rendering/engineModules/InstancingManager.hpp"

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INSTANCING GROUP
///////////////////////////////////////////////////////////////////////////////////////////

std::vector<glm::mat4> InstancingGroup::transforms() const
{
    std::vector<glm::mat4> transforms;

    for (const auto& modelObject : modelObjects)
    {
        transforms.push_back(modelObject.lock()->getModelMatrix());
    }

    return transforms;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INSTANCING MANAGER
///////////////////////////////////////////////////////////////////////////////////////////

void InstancingManager::setupInstancing(unsigned int shaderIndex, std::shared_ptr<Scene> scene)
{
    auto &modelObjects = scene->getAllObjects().models.getModels(shaderIndex);

    for (auto& modelObject : modelObjects)
    {
        if(!modelObject->enabled())
        {
            continue;
        }
        for(auto& mesh : modelObject->getModel()->meshes)
        {
            if (_instancingGroups.find(mesh._id) == _instancingGroups.end())
            {
                _instancingGroups[mesh._id] = InstancingGroup();
            }
            _instancingGroups[mesh._id].modelObjects.push_back(modelObject);
        }
    }
}   

const std::map<boost::uuids::uuid, InstancingGroup>& InstancingManager::getInstancingGroups() const
{
    return _instancingGroups;
}

void InstancingManager::resetInstancingGroups()
{
    _instancingGroups.clear();
}


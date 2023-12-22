#include "rendering/engineModules/InstancingManager.hpp"

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INSTANCING GROUP
///////////////////////////////////////////////////////////////////////////////////////////

const std::vector<glm::mat4>& InstancingGroup::transforms()
{
    _transforms.clear();

    for (const auto& modelObject : modelObjects)
    {
        _transforms.push_back(modelObject.lock()->getModelMatrix());
    }

    return _transforms;
}

InstancingGroup::~InstancingGroup()
{
    glDeleteBuffers(1, &VBO);

    if(mesh != nullptr)
    {
        //glDisableVertexArrayAttrib(mesh->VAO, 4);
        //glDisableVertexArrayAttrib(mesh->VAO, 5);
        //glDisableVertexArrayAttrib(mesh->VAO, 6);
        //glDisableVertexArrayAttrib(mesh->VAO, 7);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INSTANCING MANAGER
///////////////////////////////////////////////////////////////////////////////////////////

void InstancingManager::setupInstancing(unsigned int shaderIndex, std::shared_ptr<Scene> scene)
{
    resetInstancingGroups();

    auto &modelObjects = scene->getModels("Basic");

    for (auto& modelObject : modelObjects)
    {
        if(!modelObject->enabled())
        {
            continue;
        }
        for(auto& mesh : modelObject->getModel()->meshes)
        {
            if (_instancingGroups.find(mesh) == _instancingGroups.end())
            {
                _instancingGroups[mesh] = InstancingGroup();
                _instancingGroups[mesh].mesh = mesh;
            }
            _instancingGroups[mesh].modelObjects.push_back(modelObject);
        }
    }

    for(auto& instancingGroup : _instancingGroups)
    {
        setupInstancingGroup(instancingGroup.second);
    }
}   

const std::unordered_map<std::shared_ptr<Mesh>, InstancingGroup>& InstancingManager::getInstancingGroups() const
{
    return _instancingGroups;
}

void InstancingManager::resetInstancingGroups()
{
    _instancingGroups.clear();
}

void InstancingManager::setupInstancingGroup(InstancingGroup& group)
{
        std::shared_ptr<Mesh> mesh = group.mesh;
        const std::vector<glm::mat4>& modelMatrices = group.transforms();

        // vertex buffer object
        glGenBuffers(1, &group.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, group.VBO);
        glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

        glBindVertexArray(mesh->VAO);
        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * vec4Size));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * vec4Size));

        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);

        // Unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}
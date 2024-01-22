#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

// First party includes
#include "resources/Mesh.hpp"
#include "scene/ModelObject.hpp"
#include "scene/Scene.hpp"
#include "rendering/shader/Shader.hpp"

// STD library includes
#include <map>
#include <memory>

// Third-party headers
#include <boost/uuid/uuid.hpp>

class FluxLumina;

class InstancingGroup
{
public:
    ~InstancingGroup();
    std::vector<std::weak_ptr<ModelObject>> modelObjects;
    std::shared_ptr<Mesh> mesh;
const std::vector<glm::mat4>& transforms();
    unsigned int VBO;
    std::vector<glm::mat4> _transforms;
};

class InstancingManager
{
public:
    InstancingManager(FluxLumina* engine);
    void setupInstancing(std::shared_ptr<Scene> scene, std::shared_ptr<Shader> shader = nullptr);
    const std::unordered_map<std::shared_ptr<Mesh>, InstancingGroup>& getInstancingGroups() const;
    void resetInstancingGroups();

private:
    void setupInstancingGroup(InstancingGroup& instancingGroup);

    // Mesh uuid -> InstancingGroup
    std::unordered_map<std::shared_ptr<Mesh>, InstancingGroup> _instancingGroups;

    FluxLumina* _ranFrom;
};

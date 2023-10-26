#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.h"

// GLM - Math is a gateway science
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// First party includes
#include "rendering/Mesh.h"
#include "scene/ModelObject.h"
#include "scene/Scene.h"

// STD library includes
#include <map>
#include <memory>

// Third-party headers
#include <boost/uuid/uuid.hpp>


struct InstancingGroup
{
    std::vector<std::weak_ptr<ModelObject>> modelObjects;
    std::vector<glm::mat4> transforms() const;
};

class InstancingManager
{
public:
    void setupInstancing(unsigned int shaderIndex, std::shared_ptr<Scene> scene);
    const std::map<boost::uuids::uuid, InstancingGroup>& getInstancingGroups() const;
    void resetInstancingGroups();

private:
    // Mesh uuid -> InstancingGroup
    std::map<boost::uuids::uuid, InstancingGroup> _instancingGroups;
};

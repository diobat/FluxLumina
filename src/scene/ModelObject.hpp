#pragma once

// First-party headers
#include "scene/sceneObject.hpp"
#include "resources/Texture.hpp"
#include "resources/Model.hpp"
#include "util/Listener.hpp"
#include "scene/ModelMatrixProvider.hpp"

// Third-party headers
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


class ModelObject : public SceneObject, public std::enable_shared_from_this<ModelObject>
{
public:
    ModelObject();

    void setModel(const std::shared_ptr<Model> &model);
    std::shared_ptr<Model> getModel();

    void setShaderIndex(unsigned int shaderIndex);
    unsigned int getShaderIndex() const;

    const glm::mat4& getModelMatrix();

    Broadcaster& getBroadcaster();

    const boost::uuids::uuid& uuid() const;

private:
    // Structural Data
    boost::uuids::uuid _id;

    std::shared_ptr<Model> _model;
    unsigned int _shaderIndex;
    ModelMatrixProvider _modelMatrixProvider;

    Broadcaster _broadcaster;
};


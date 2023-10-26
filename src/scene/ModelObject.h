#pragma once

// First-party includes
#include "scene/sceneObject.h"
#include "rendering/texture.h"
#include "rendering/model.h"
#include "util/Listener.h"
#include "scene/ModelMatrixProvider.h"

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


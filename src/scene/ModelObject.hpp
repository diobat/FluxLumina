#pragma once

// First-party headers
#include "scene/SceneObject.hpp"
#include "resources/Texture.hpp"
#include "resources/Model.hpp"
// #include "util/Listener.hpp"
#include "scene/ModelMatrixProvider.hpp"

// Third-party headers
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


/*
    The engine will use this enum to determine how to draw the model.
*/
enum class drawMode
{
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};


class ModelObject : public SceneObject, public std::enable_shared_from_this<ModelObject>
{
public:
    ModelObject();

    void setModel(const std::shared_ptr<Model> &model);
    std::shared_ptr<Model> getModel();

    void setShaderIndex(unsigned int shaderIndex);
    unsigned int getShaderIndex() const;
    void setShaderName(const std::string& shaderName);
    const std::string& getShaderName() const;

    const glm::mat4& getModelMatrix();

    const boost::uuids::uuid& uuid() const;

private:
    // Structural Data
    boost::uuids::uuid _id;
    


    std::string _assignedShader;

    std::shared_ptr<Model> _model;
    unsigned int _shaderIndex;
    ModelMatrixProvider _modelMatrixProvider;

};


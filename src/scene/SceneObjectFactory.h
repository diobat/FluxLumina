#pragma once

// First-party includes
#include "scene/Scene.h"
#include "scene/SceneObject.h"
#include "rendering/GraphicalEngine.h"
#include "scene/ModelObject.h"
#include "scene/Camera.h"
#include "scene/LightSource.h"
#include "rendering/Cubemap.h"

// STD library includes
#include <iostream>

//Third-party includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*
    This is the class that manages the proper creation and release of resources for rendered objects
    It manages safe coordination between the scene and rendering engine while allowing the first to not be aware of the latter
*/

class SceneObjectFactory
{
public:
    SceneObjectFactory(Scene* scene = nullptr, GraphicalEngine* engine = nullptr);

    void bindScene(Scene* scene);
    void bindEngine(GraphicalEngine* engine);

    // Importing models
    ModelObject &create_Model(const std::string &modelPath, unsigned int shader = 0, bool flipUVs = false);
    void load_ModelMeshes(Model& model, std::string const &path);
    void processNode(Model& model, aiNode* node, const aiScene* scene);
    Mesh processMesh(Model& model, aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(Model &model, aiMaterial *mat, aiTextureType type);

    // Creating lights
    std::shared_ptr<LightSource> create_LightSource(E_LightType type);

    // Creating cameras
    void create_Camera();

    // Creating cubemaps
    std::shared_ptr<Cubemap> create_Skybox(std::vector<std::string> faces);

private:
    Scene* _boundScene;
    GraphicalEngine* _boundEngine;
};

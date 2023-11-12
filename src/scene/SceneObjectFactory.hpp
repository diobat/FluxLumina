#pragma once

// STL library includes
#include <iostream>

// First-party includes
#include "scene/Scene.hpp"
#include "scene/SceneObject.hpp"
#include "rendering/GraphicalEngine.hpp"
#include "scene/ModelObject.hpp"
#include "scene/Camera.hpp"
#include "scene/LightSource.hpp"
#include "resources/Cubemap.hpp"
#include "rendering/MeshLibrary.hpp"

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
    SceneObjectFactory(
        Scene* scene = nullptr, 
        GraphicalEngine* engine = nullptr,
        MeshLibrary* meshLibrary = nullptr);

    void bindScene(Scene* scene);
    void bindEngine(GraphicalEngine* engine);

    // Importing models
    ModelObject &create_Model(const std::string &modelPath, unsigned int shader = 0, bool flipUVs = false);
    void load_ModelMeshes(Model& model, std::string const &path);
    void processNode(const std::string &path, aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(const std::string &path, aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const std::string &path, aiMaterial *mat, aiTextureType type);

    // Creating lights
    std::shared_ptr<LightSource> create_LightSource(E_LightType type);

    // Creating cameras
    void create_Camera();

    // Creating cubemaps
    std::shared_ptr<Cubemap> create_Skybox(std::vector<std::string> faces);

private:
    Scene* _boundScene;
    GraphicalEngine* _boundEngine;
    MeshLibrary* _meshLibrary;
};

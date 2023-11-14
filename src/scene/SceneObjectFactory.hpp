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
    Some texture types are not supported by the formats our engine uses
    This means than we must externally supply the location of the texture
    This struct is used to store the location of the texture, and the type of texture it is
    An instance of this struct can then be (optionally) passed to the SceneObjectFactory during the creation of a model.
*/
struct TextureLocations
{
    std::vector<std::string> heightMaps;
};

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
    ModelObject &create_Model(const std::string &modelPath, unsigned int shader = 0, bool flipUVs = false, TextureLocations textureLocations = {});
    void load_ModelMeshes(Model& model, std::string const &path);
    void processNode(const std::string &path, aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(const std::string &path, aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiScene* scene, const std::string &path, aiMaterial *mat, aiTextureType type);
    std::vector<Texture> loadExternalTextures(const std::string &path, const TextureLocations & textures) const;

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

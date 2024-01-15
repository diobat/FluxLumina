#pragma once

#include <string>
#include <vector>
#include <memory>

// Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Scene;
class GraphicalEngine;
class LightSource;
class Cubemap;
class Texture;
class Model;
class ModelObject;
class Mesh;
struct Vertex;
enum E_LightType;

/*
    Some texture types are not supported by the formats our engine uses
    This means than we must externally supply the location of the texture
    This struct is used to store the location of the texture, and the type of texture it is
    An instance of this struct can then be (optionally) passed to the SceneObjectFactory during the creation of a model.
*/
struct TextureLocations
{
    TextureLocations(std::vector<std::string> heightMaps = {}, std::vector<std::string> lightMaps = {}) : heightMaps(heightMaps), lightMaps(lightMaps) {};

    std::vector<std::string> heightMaps;
    std::vector<std::string> lightMaps;
};

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

    // Importing models via assimp
    ModelObject &create_Model(const std::string &modelPath, const std::string& shader = "Basic", bool flipUVs = false, TextureLocations textureLocations = {});

    // Importing models manually (not recommended)
    ModelObject &create_Model(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& shader);

    // Creating lights
    std::shared_ptr<LightSource> create_LightSource(E_LightType type);

    // Creating cameras
    void create_Camera(float fov = 70.0f, float translationSpeed = 0.2f, float rotationSpeed = 0.001f);

    // Creating cubemaps
    std::shared_ptr<Cubemap> create_Skybox(std::vector<std::string> faces, bool flipUVs = false);

    // Creating IBL textures
    std::shared_ptr<Cubemap> create_IBL(std::string path, bool flipUVs = false);

private:
    void load_ModelMeshes(Model& model, std::string const &path);
    void processNode(const std::string &path, aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> processMesh(const std::string &path, aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiScene* scene, const std::string &path, aiMaterial *mat, aiTextureType type);
    std::vector<Texture> loadExternalTextures(const std::string &path, const TextureLocations & textures) const;

    Scene* _boundScene;
    GraphicalEngine* _boundEngine;
};

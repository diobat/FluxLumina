#include "scene/SceneObjectFactory.h"

#include "helpers/RootDir.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// HELPER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////

namespace
{

    std::vector<Texture> loadedTextures;

    bool flipUVsOnLoad = false;

    Texture TextureFromFile(const char *path, const std::string &directory, bool gamma = false)
    {
        std::string filename = std::string(path);
        filename = ROOT_DIR + directory + '/' + filename;

        stbi_set_flip_vertically_on_load(flipUVsOnLoad);

        Texture texture;
        texture._pixels = stbi_load(filename.c_str(), &texture._width, &texture._height, &texture._components, 0);
        texture._path = path;

        return texture;
    }

    void setTextureData(Texture &texture, aiTextureType type)
    {

        // Set texture parameters
        if (texture._components == 1)
            texture._colorChannels = GL_RED;
        else if (texture._components == 3)
            texture._colorChannels = GL_RGB;
        else if (texture._components == 4)
            texture._colorChannels = GL_RGBA;

        switch(type)
        {
            case aiTextureType_DIFFUSE:
                texture._type = E_TexureType::DIFFUSE;
                break;
            case aiTextureType_SPECULAR:
                texture._type = E_TexureType::SPECULAR;
                break;
            case aiTextureType_NORMALS:
                texture._type = E_TexureType::NORMAL;
                break;
            case aiTextureType_HEIGHT:
                texture._type = E_TexureType::HEIGHT;
                break;
            default:
                texture._type = E_TexureType::DIFFUSE;
                break;
        }

        texture._useLinear = true;
    }

    void TextureData_Free(Texture &texture)
    {
        stbi_image_free(texture._pixels);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INITIALIZATION
///////////////////////////////////////////////////////////////////////////////////////////

SceneObjectFactory::SceneObjectFactory(Scene* scene, GraphicalEngine* engine)
{
    _boundScene = scene;
    _boundEngine = engine;

    if(scene != nullptr)
    {
        ModelObject& cube = create_Model("res/models/origin_cube.obj");
        cube.setScale(0.3f);
    }
}

void SceneObjectFactory::bindScene(Scene* scene)
{
    _boundScene = scene;
}

void SceneObjectFactory::bindEngine(GraphicalEngine *engine)
{
    _boundEngine = engine;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// MODELS/MESHES/MATERIALS
///////////////////////////////////////////////////////////////////////////////////////////

ModelObject &SceneObjectFactory::create_Model(const std::string &modelPath, unsigned int shader, bool flipUVs)
{
    std::shared_ptr<ModelObject> model_object = std::make_shared<ModelObject>();
    Model& model = (*model_object->getModel());

    // Set the shader in position 0 as the default shader
    model_object->setShaderIndex(_boundEngine->getShaderProgramID(shader));

    flipUVsOnLoad = flipUVs;

    load_ModelMeshes(model, modelPath);
    for (auto &one_mesh : model.meshes)
    {
        _boundEngine->initializeMesh(one_mesh);
    }

    _boundScene->addModel(model_object);

    return *model_object;
}

void SceneObjectFactory::load_ModelMeshes(Model& model, const std::string& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(ROOT_DIR + path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    model.directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively to build the meshes
    processNode(model, scene->mRootNode, scene);

}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void SceneObjectFactory::processNode(Model &model, aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        model.meshes.push_back(processMesh(model, mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(model, node->mChildren[i], scene);
    }
}

Mesh SceneObjectFactory::processMesh(Model &model, aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;


    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        
        // colors
        aiColor4D diffuse;
        aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];

        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        {
            vector.x = diffuse.r;
            vector.y = diffuse.g;
            vector.z = diffuse.b;
        }
        else
        {
            vector.x = 1.0f;
            vector.y = 1.0f;
            vector.z = 1.0f;
        }
        vertex.Color = vector;

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(model, material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(model, material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        float opacity = 1.0f;
        if(material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
        {
            if(opacity < 1.0f)
            {
                model.hasTransparency = true;
            }
        }
    

    }
    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> SceneObjectFactory::loadMaterialTextures(Model &model, aiMaterial *mat, aiTextureType type)
{
    std::vector<Texture> materialTextures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool isPreviouslyLoaded = false;

        for(unsigned int k = 0; k < loadedTextures.size(); k++)
        {
            if(std::strcmp(loadedTextures[k]._path.data(), str.C_Str()) == 0)
            {
                materialTextures.push_back(loadedTextures[k]);
                isPreviouslyLoaded = true;
                break;
            }
        }

        if(!isPreviouslyLoaded)
        {

            Texture texture = TextureFromFile(str.C_Str(), model.directory);
            setTextureData(texture, type);

            _boundEngine->initializeTexture(texture);
            materialTextures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }
    return materialTextures;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHT SOURCES
///////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<LightSource> SceneObjectFactory::create_LightSource(E_LightType type)
{
    std::shared_ptr<LightSource> light;

    switch(type)
    {
        case E_LightType::DIRECTIONAL_LIGHT:
            light = std::make_shared<DirectionalLight>();
            break;
        case E_LightType::POINT_LIGHT:
            light = std::make_shared<PointLight>();
            break;
        case E_LightType::SPOT_LIGHT:
            light = std::make_shared<SpotLight>();
            break;
        default:
            light = nullptr;
            return light;
            break;
    }

    _boundScene->addLightSource(light);

    return light;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// CAMERAS
///////////////////////////////////////////////////////////////////////////////////////////

void SceneObjectFactory::create_Camera()
{
    _boundScene->addCamera(std::make_shared<Camera>());
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// CUBEMAPS
///////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Cubemap> SceneObjectFactory::create_Skybox(std::vector<std::string> faces)
{
    std::array<Texture,6> cubemapTex; 

    if(faces.size() != 6)
    {
        return nullptr;
    }

    for(unsigned int i = 0; i < faces.size(); i++)
    {
        Texture texture = TextureFromFile(faces[i].c_str(), "");
        setTextureData(texture, aiTextureType_DIFFUSE);
        cubemapTex[i] = texture;
       
        if(!*texture._pixels)
        {
            std::cout << "ERROR::CUBEMAP::TEXTURE_LOADING_FAILED " << i << std::endl;
        }
    }
    std::shared_ptr<Cubemap> cubemap = std::make_shared<Cubemap>();
    cubemap->getTexture()._type = E_TexureType::CUBEMAP;
    _boundScene->getSkybox().setCubemap(cubemap);
    _boundEngine->initializeSkybox(_boundScene->getSkybox(), cubemapTex);

    return cubemap;
}
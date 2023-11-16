#include "scene/SceneObjectFactory.hpp"

#include "helpers/RootDir.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// HELPER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////



namespace
{
    enum class FileType
    {
        NONE,
        OBJ,
        GLB,
        UNKNOWN
    } fileType;

    TextureLocations externalTextureLocations;
    bool flipUVsOnLoad = false;

    void detectFileType(const std::string& path)
    {
        std::string extension = path.substr(path.find_last_of('.') + 1);
        if(extension == "OBJ" || extension == "obj")
        {
            fileType = FileType::OBJ;
        }
        else if(extension == "GLB" || extension == "glb")
        {
            fileType = FileType::GLB;
        }
        else
        {
            fileType = FileType::UNKNOWN;
        }
    }

    Texture TextureFromFile(const char* path, const std::string &directory)
    {
        std::string filename = std::string(path);
        filename = ROOT_DIR + directory + '/' + filename;

        stbi_set_flip_vertically_on_load(flipUVsOnLoad);

        Texture texture;
        texture._pixels = stbi_load(filename.c_str(), &texture._width, &texture._height, &texture._components, 0);
        texture._path = directory + '/' + path;

        return texture;
    }

    Texture TextureFromEmbeddedTexture(const aiTexture* texture, const std::string &directory = "")
    {

        stbi_set_flip_vertically_on_load(false);

        Texture embeddedTexture;
        embeddedTexture._pixels = stbi_load_from_memory(
            reinterpret_cast<const unsigned char*>(texture->pcData),
            texture->mWidth,
            &embeddedTexture._width,
            &embeddedTexture._height,
            &embeddedTexture._components,
            0);
        embeddedTexture._path = directory + '/' + std::string(texture->mFilename.C_Str()) + "." + texture->achFormatHint;

        return embeddedTexture;
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
                if (fileType == FileType::OBJ)
                {   // Wavefront misclassifies normal maps as height maps
                    texture._type = E_TexureType::NORMAL;
                }
                else
                {
                    texture._type = E_TexureType::HEIGHT;
                }
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

SceneObjectFactory::SceneObjectFactory(Scene* scene, GraphicalEngine* engine, MeshLibrary* meshLibrary)
{
    _boundScene = scene;
    _boundEngine = engine;
    _meshLibrary = meshLibrary;

    if(scene != nullptr)
    {
        ModelObject& cube = create_Model("res/models/origin_cube.obj", 0);
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

ModelObject &SceneObjectFactory::create_Model(const std::string &modelPath, unsigned int shader, bool flipUVs, TextureLocations textureLocations)
{
    detectFileType(modelPath);

    std::shared_ptr<ModelObject> model_object = std::make_shared<ModelObject>();
    Model& model = (*model_object->getModel());

    // Set the shader program
    model_object->setShaderIndex(_boundEngine->getShaderProgramID(shader));

    // Set the per-model creation variables
    flipUVsOnLoad = flipUVs;
    externalTextureLocations = textureLocations;

    load_ModelMeshes(model, modelPath);

    _boundScene->addModel(model_object);

    fileType = FileType::NONE;
    return *model_object;
}

void SceneObjectFactory::load_ModelMeshes(Model& model, const std::string& path)
{

    // check if model is already loaded
    if(_meshLibrary != nullptr && !_meshLibrary->isMeshLoaded(path))
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(ROOT_DIR + path, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs | 
        //aiProcess_JoinIdenticalVertices |
        //aiProcess_FixInfacingNormals | 
        aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }
        // retrieve the directory path of the filepath
        model.directory = path;

        // process ASSIMP's root node recursively to build the meshes
        processNode(model.directory, scene->mRootNode, scene);
    }

    model.directory = path;
    model.meshes = _meshLibrary->getMeshes(path);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void SceneObjectFactory::processNode(const std::string &path, aiNode *node, const aiScene *scene)
{
    std::vector<std::shared_ptr<Mesh>> newMeshes;

    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        newMeshes.push_back(processMesh(path, mesh, scene));
    }
    
    for(auto& mesh : newMeshes)
    {
        _boundEngine->initializeMesh(mesh);
    }
    _meshLibrary->addMesh(path, newMeshes);

    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(path, node->mChildren[i], scene);
    }

}

std::shared_ptr<Mesh> SceneObjectFactory::processMesh(const std::string &path, aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    bool hasTransparency = false;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        
        // Normals
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
        
        // Colors
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

        // Tangents
        if(mesh->HasTangentsAndBitangents())
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
        }
        else
        {
            vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
        }

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

    std::string directory = path.substr(0, path.find_last_of('/'));

    // process materials
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(scene, directory, material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(scene, directory, material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        if(fileType == FileType::OBJ)
        {
            std::vector<Texture> normalMaps = loadMaterialTextures(scene, directory, material, aiTextureType_HEIGHT);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }
        else
        {
            std::vector<Texture> normalMaps = loadMaterialTextures(scene, directory, material, aiTextureType_NORMALS);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }
        
        float opacity = 1.0f;
        if(material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
        {
            if(opacity < 1.0f)
            {
                hasTransparency = true;
            }
        }
    }

    // process external materials
    std::vector<Texture> externalMaps = loadExternalTextures(directory, externalTextureLocations);
    textures.insert(textures.end(), externalMaps.begin(), externalMaps.end());

    // return a mesh object created from the extracted mesh data
    return std::make_shared<Mesh>(Mesh(vertices, indices, textures, hasTransparency));
}

std::vector<Texture> SceneObjectFactory::loadMaterialTextures(const aiScene* scene, const std::string &path, aiMaterial *mat, aiTextureType type)
{
    std::vector<Texture> materialTextures;
    const auto& loadedTextures = _meshLibrary->getLoadedTextures();

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool isPreviouslyLoaded = false;
        //mat->Get(AI_MATKEY_TEXTURE(type, i), str);
        std::string texturePath = path + '/' + std::string(str.C_Str());
        for(unsigned int k = 0; k < loadedTextures.size(); k++)
        {
            if(std::strcmp(loadedTextures[k]._path.data(), texturePath.c_str()) == 0)
            {
                materialTextures.push_back(loadedTextures[k]);
                isPreviouslyLoaded = true;
                break;
            }
        }

        if(!isPreviouslyLoaded)
        {
            Texture texture;

            switch(fileType)
            {
                case FileType::OBJ:
                    texture = TextureFromFile(str.C_Str(), path);
                    break;
                case FileType::GLB:
                    texture = TextureFromEmbeddedTexture(scene->GetEmbeddedTexture(str.C_Str()), path);
                    break;
            }

            setTextureData(texture, type);
            _boundEngine->initializeTexture(texture);
            materialTextures.push_back(texture);
            _meshLibrary->addTexture(texture);
        }
    }
    return materialTextures;
}

std::vector<Texture> SceneObjectFactory::loadExternalTextures(const std::string &path, const TextureLocations & textures) const
{
    std::vector<Texture> materialTextures;
    const auto& loadedTextures = _meshLibrary->getLoadedTextures();

    for(unsigned int i = 0; i < textures.heightMaps.size(); i++)
    {
        bool isPreviouslyLoaded = false;

        for(unsigned int k = 0; k < loadedTextures.size(); k++)
        {
            if(std::strcmp(loadedTextures[k]._path.data(), textures.heightMaps[i].c_str()) == 0)
            {
                materialTextures.push_back(loadedTextures[k]);
                isPreviouslyLoaded = true;
                break;
            }
        }

        if(!isPreviouslyLoaded)
        {
            Texture texture = TextureFromFile(textures.heightMaps[i].c_str(), path);
            setTextureData(texture, aiTextureType_HEIGHT);
            texture._type = E_TexureType::HEIGHT;   // Wavefront misclassifies normal maps as height maps, temporarily fix
            _boundEngine->initializeTexture(texture);
            materialTextures.push_back(texture);
            _meshLibrary->addTexture(texture);
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
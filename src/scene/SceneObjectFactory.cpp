#include "scene/SceneObjectFactory.h"

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

ModelObject &SceneObjectFactory::create_Model(const std::string &modelPath, const std::string &texturePath)
{
    std::shared_ptr<ModelObject> model_object = std::make_shared<ModelObject>();
    Model& model = (*model_object->getModel());

    load_Model(model, modelPath);

    for (auto &one_mesh : model.meshes)
    {
        _boundEngine->initializeMesh(one_mesh);
    }

    _boundScene->addModel(model_object);
    return *model_object;
}

void SceneObjectFactory::load_Model(Model& model, const std::string& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(ROOT_DIR + path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    model.directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
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

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices);
}

void SceneObjectFactory::create_LightSource()
{

}

void SceneObjectFactory::create_Camera()
{
    _boundScene->addCamera(std::make_shared<Camera>());
}
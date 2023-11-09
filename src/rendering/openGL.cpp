#include "rendering/openGL.hpp"

GLFWwindow* openGL::getWindowPtr()
{
    return _window;
}

void openGL::enable(GLuint feature)
{
    glEnable(feature);
}

void openGL::disable(GLuint feature)
{
    glDisable(feature);
}

int openGL::initialize(GLFWwindow* window)
{
    _window = window;

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    glfwGetWindowSize(_window, &_viewportWidth, &_viewportHeight);

    /* Set the viewport */
    glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);
    glViewport(0, 0, _viewportWidth, _viewportHeight);

    // Enable face culling
    glEnable(GL_CULL_FACE);

    // Enable 16xMSAA
    glEnable(GL_MULTISAMPLE);

    // Enable Gamma Correction
    //glEnable(GL_FRAMEBUFFER_SRGB); 

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set window resize code
    glfwSetWindowUserPointer(_window, this);
    auto func = [](GLFWwindow* window, int width, int height)
    {
        static_cast<openGL*>(glfwGetWindowUserPointer(window))->resizeWindow(window, width, height);
    };
    glfwSetWindowSizeCallback(_window, func);
    // End window resize code


    // Framebuffer Manager initialization
    _frameBuffers = std::make_shared<FBOManager>(_window);

    // Shader Library initialization
    _shaderPrograms = std::make_shared<ShaderLibrary>();

    _shaderPrograms->addShader("Basic.vert", "Basic.frag");                  //0
    _shaderPrograms->getShader(0)->addSupportedFeature(E_ShaderProgramFeatures::E_AUTO_INSTANCING);
    _shaderPrograms->addShader("Simple.vert", "Simple.frag");                //1
    _shaderPrograms->addShader("Simple.vert", "transparency.frag");          //2
    _shaderPrograms->getShader(2)->addSupportedFeature(E_ShaderProgramFeatures::E_TRANSPARENCY);
    _shaderPrograms->addShader("Quad.vert", "Quad.frag");                    //3
    _shaderPrograms->addShader("Skybox.vert", "Skybox.frag");                //4
    _shaderPrograms->addShader("Reflection.vert", "Reflection.frag");        //5
    _shaderPrograms->addShader("ShadowMap.vert", "ShadowMap.frag");          //6
    _shaderPrograms->getShader(6)->addSupportedFeature(E_ShaderProgramFeatures::E_SHADOW_MAPPING);
    _shaderPrograms->addShader("ShadowCubeMap.vert", "ShadowCubeMap.frag", "ShadowCubeMap.geom");      //7
    _shaderPrograms->getShader(7)->addSupportedFeature(E_ShaderProgramFeatures::E_SHADOW_CUBE_MAPPING);
    _shaderPrograms->use(0);

    // Add uniform buffers to the shaders
    _shaderPrograms->createUniformBuffer("mvp_camera");
    _shaderPrograms->createUniformBuffer("viewPosBlock");

    // Initialize LightManager
    _lightLibrary = std::make_shared<LightLibrary>();
    _lightLibrary->bindFramebufferManager(_frameBuffers);
    _lightLibrary->bindShaderLibrary(_shaderPrograms);

    // Initialize Instancing Manager
    _instancingManager = std::make_shared<InstancingManager>();

    // Initialize rendering strategy
    _strategyChain = std::make_shared<DefaultStrategyChain>(this);

    return 1;
}

// Next step is to encapsulate this in a method that also handles Framebuffer changes
void openGL::renderFrame(std::shared_ptr<Scene> scene)
{

    _strategyChain->run();
//     // Camera
//     _shaderPrograms->use(0);
//     cameraSetup(scene);

//     // Light
//     _shaderPrograms->use(0);
//     _lightLibrary->alignShadowMaps(scene);
//     _lightLibrary->renderShadowMaps(scene);
//     _shaderPrograms->use(0);
//     _lightLibrary->prepare(scene->getAllLights());

//     // Bind the proper FBO
//     _frameBuffers->bindProperFBOFromScene(scene);
//     // Reset color and depth buffers 
//     _frameBuffers->clearAll();

//     // Adjust ViewPort
//     glViewport(0, 0, _viewportWidth, _viewportHeight);

//     // Draw models
//     std::set <unsigned int> shaderIndexes = _shaderPrograms->getShaderIndexesPerFeature();
//     std::set <unsigned int> shaderIndexesWithInstancing = _shaderPrograms->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_AUTO_INSTANCING);
//     std::set<unsigned int> shaderIndexesTotal;
//     std::merge(shaderIndexes.begin(), shaderIndexes.end(), shaderIndexesWithInstancing.begin(), shaderIndexesWithInstancing.end(), std::inserter(shaderIndexesTotal, shaderIndexesTotal.begin()));

//     // Opaque Models
//     for(unsigned int shaderIndex : shaderIndexesTotal)
//     {
//         // Activate shader
//         _shaderPrograms->use(shaderIndex);

//         // Go down the instancing path if the shader supports it
//         if(_shaderPrograms->getShader(shaderIndex)->isFeatureSupported(E_ShaderProgramFeatures::E_AUTO_INSTANCING))
//         {
//             renderInstancedMeshes();
//         }
//         else // Else just render on a per-model basis
//         {
//             for (auto model : scene->getModels(_shaderPrograms->getShader(shaderIndex)->getProgramId()))
//             {
//                 renderModel(*model);   
//             }
//         }
//     }

//     //Draw Skybox
//     if (scene->getSkybox().getCubemap() != nullptr)
//     {
//         auto ZZview = glm::mat4(glm::mat3(scene->getActiveCamera()->getViewMatrix())); // remove translation from the view matrix
//         auto ZZprojection = scene->getActiveCamera()->getProjectionMatrix();
//         _shaderPrograms->use(4);
//         _shaderPrograms->setUniformMat4(4, "view", ZZview);
//         _shaderPrograms->setUniformMat4(4, "projection", ZZprojection);
//         renderSkybox(scene->getSkybox());
//     }

//     // Transparent Models
//     for(unsigned int shaderIndex : _shaderPrograms->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_TRANSPARENCY))
//     {
//         // Activate shader
//         _shaderPrograms->use(shaderIndex);
//         std::map<float, std::shared_ptr<ModelObject>> sortedTransparentModels;
        
//         for (auto model : scene->getModels(_shaderPrograms->getShader(shaderIndex)->getProgramId()))
//         {
//             if(model->getModel()->hasTransparency())
//             {
//                 float distance = glm::length(scene->getActiveCamera()->getPosition() - conversion::toVec3(model->getPosition()));
//                 sortedTransparentModels[distance] = model;
//             }
//         }
//         // Render all the transparent models (from in decreasing distance to the camera)
//         if(!sortedTransparentModels.empty())
//         {
//             for (std::map<float, std::shared_ptr<ModelObject>>::reverse_iterator it = sortedTransparentModels.rbegin(); it != sortedTransparentModels.rend(); ++it)
//             {
//                 renderModel(*it->second);
//             }
//         }
//     }
}

void openGL::renderModel(ModelObject &model)
{
    _shaderPrograms->setUniformMat4("model", model.getModelMatrix());

    for (auto &one_mesh : model.getModel()->meshes)
    {
        bindTextures(one_mesh);
        glBindVertexArray(one_mesh->VAO);
        int vertexCount = static_cast<int>(one_mesh->_indices.size());
        glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    }
}

void openGL::renderInstancedMeshes(std::shared_ptr<InstancingManager> instancingManager)
{
    // In case no argument is passed just use the default instancing manager
    if (instancingManager == nullptr)
    {
        instancingManager = _instancingManager;
    }

    for(auto& instancingGroup : _instancingManager->getInstancingGroups())
    {
        bindTextures(instancingGroup.second.mesh); 
        glBindVertexArray(instancingGroup.second.mesh->VAO);
        int vertexCount = static_cast<int>(instancingGroup.second.mesh->_indices.size());
        int instanceCount = static_cast<int>(instancingGroup.second.modelObjects.size());
        glDrawElementsInstanced(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0, instanceCount);
    }
    // Unbind the VAO
    glBindVertexArray(0);
}

// void openGL::cameraSetup(std::shared_ptr<Scene> scene)
// {
//     // Camera
//     scene->getActiveCamera()->recalculateMVP();

//     std::tuple<glm::mat4, glm::mat4> mvp = {
//         scene->getActiveCamera()->getViewMatrix(),
//         scene->getActiveCamera()->getProjectionMatrix()
//         };

//     std::tuple<glm::vec3> cameraPosition = {
//         scene->getActiveCamera()->getPosition()
//     };

//     _shaderPrograms->getUniformBuffer("mvp_camera").update(mvp);
//     _shaderPrograms->getUniformBuffer("viewPosBlock").update(cameraPosition);
// }

void openGL::resizeWindow(GLFWwindow* window, int width, int height)
{
    _viewportWidth = width;
    _viewportHeight = height;
    glViewport(0, 0, _viewportWidth, _viewportHeight);

    for(auto scene : _scenes)
    {
        if (scene->getActiveCamera() != nullptr)
        {
            scene->getActiveCamera()->resizeCameraPlane((float)width, (float)height);
        }
    }
}

void openGL::initializeMesh(std::shared_ptr<Mesh>& mesh)
{

    // create buffers/arrays
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->_vertices.size() * sizeof(Vertex), &mesh->_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->_indices.size() * sizeof(unsigned int), &mesh->_indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // vertex color info
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color));

    glBindVertexArray(0);
}

void openGL::initializeTexture(Texture& texture)
{
    // load and create a texture
    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_2D, texture._id);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //glTexStorage2D(GL_TEXTURE_2D, 2 /* mip map levels */, GL_RGB8, texture._width, texture._height);
    glTexImage2D(GL_TEXTURE_2D, 0, texture._colorChannels, texture._width, texture._height, 0, texture._colorChannels, GL_UNSIGNED_BYTE, texture._pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}   

void openGL::bindTextures(std::shared_ptr<Mesh> mesh)
{
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int cubemapNr = 0;

    _shaderPrograms->setUniformInt("sampleFromDiffuse", 0);
    _shaderPrograms->setUniformInt("sampleFromSpecular", 0);

    if (mesh->_textures.size() == 0)
    {
        return;
    }

    int imageUnitSpace;

    for (int i = 0; i < mesh->_textures.size(); i++)
    {

        switch (mesh->_textures[i]._type)
        {
        case DIFFUSE:
            imageUnitSpace = 0;
            _shaderPrograms->setUniformInt("sampleFromDiffuse", 1);
            _shaderPrograms->setUniformInt("material.diffuse", 1);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            diffuseNr++;
            break;
        case SPECULAR:
            imageUnitSpace = 20;
            _shaderPrograms->setUniformInt("sampleFromSpecular", 1);
            _shaderPrograms->setUniformInt("material.specular", 2);
            _shaderPrograms->setUniformFloat("material.shininess", 4.5f);
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            specularNr++;
            break;
        case NORMAL:
            imageUnitSpace = 40;
            break;
        case HEIGHT:
            imageUnitSpace = 60;
            break;
        case CUBEMAP:
            imageUnitSpace = 80;
            _shaderPrograms->setUniformInt("cubemap", 3);
            glActiveTexture(GL_TEXTURE0 + 3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mesh->_textures[i]._id);
            cubemapNr++;
            break;
        default:
            imageUnitSpace = 0;
            break;
        }

    }
}

unsigned int openGL::getShaderProgramID(unsigned int shaderIndex)
{
    return _shaderPrograms->getShader(shaderIndex)->getProgramId();
}

std::shared_ptr<FBO> openGL::addFBO(E_AttachmentFormat format, int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        glfwGetWindowSize(_window, &width, &height);
    }

    std::shared_ptr<FBO> fbo = _frameBuffers->addFBO(format, width, height);
    return fbo;
}

void openGL::bindFBO(unsigned int fboIndex)
{
    _frameBuffers->bindFBO(fboIndex);
}

void openGL::unbindFBO()
{
    _frameBuffers->unbindFBO();
}

unsigned int openGL::getFBOIndex(std::shared_ptr<FBO> fbo) const
{
    return _frameBuffers->getFBOIndex(fbo);
}

bool openGL::isFrameBufferComplete(std::shared_ptr<FBO> fbo) const
{
    return _frameBuffers->isFrameBufferComplete(fbo);
}

FBOManager& openGL::getFBOManager()
{
    return *_frameBuffers;
}

void openGL::initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures)
{
    Cubemap& cubemap = *skybox.getCubemap();
    // VAO and VBO
    glGenVertexArrays(1, &cubemap.VAO);
    glGenBuffers(1, &cubemap.VBO);
    glBindVertexArray(cubemap.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap.VBO);

    auto vertices = *cubemap._vertices;

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubemap._vertices), &cubemap._vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO

    // Setup textures of the cubemap

    Texture& cubemapTexture = cubemap.getTexture();
    glGenTextures(1, &cubemapTexture._id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture._id);
    // load and create a texture
    for (int i(0); i < textures.size() ; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textures[i]._colorChannels, textures[i]._width, textures[i]._height, 0, textures[i]._colorChannels, GL_UNSIGNED_BYTE, textures[i]._pixels);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // Unbind texture

}

void openGL::renderSkybox(Skybox& skybox)
{
    auto cubemap = skybox.getCubemap();

    //glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(cubemap->VAO);
    _shaderPrograms->setUniformInt("skyboxCube", 30);
    glActiveTexture(GL_TEXTURE0 + 30);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->getTexture()._id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glDepthFunc(GL_LESS);
    //glDepthMask(GL_TRUE);

}

void openGL::initializeInstanceManager(std::shared_ptr<Scene> scene)
{
    _instancingManager->setupInstancing(_shaderPrograms->getShader(0)->getProgramId(), _scenes[0]);
}


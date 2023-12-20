#include "rendering/openGL.hpp"

GLFWwindow* openGL::getWindowPtr()
{
    return _window;
}

int openGL::initialize(GLFWwindow* window, E_RenderStrategy strategy)
{
    _window = window;

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    glfwGetWindowSize(_window, &_viewportWidth, &_viewportHeight);

    /* Set the viewport */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, _viewportWidth, _viewportHeight);

    // Set window resize code
    glfwSetWindowUserPointer(_window, this);
    auto func = [](GLFWwindow* window, int width, int height)
    {
        static_cast<openGL*>(glfwGetWindowUserPointer(window))->resizeWindow(window, width, height);
    };
    glfwSetWindowSizeCallback(_window, func);
    // End window resize code

    // Settings module initialization
    _settings = std::make_shared<Settings>(_window);

    // Framebuffer Manager initialization
    _frameBuffers = std::make_shared<FBOManager>(this);

    // Shader Library initialization
    _shaderPrograms = std::make_shared<ShaderLibrary>("res/shaders/");

    _shaderPrograms->getShader("Basic")->addSupportedFeature(E_ShaderProgramFeatures::E_AUTO_INSTANCING);
    _shaderPrograms->getShader("transparency")->addSupportedFeature(E_ShaderProgramFeatures::E_TRANSPARENCY);

    // Add uniform buffers to the shaders
    _shaderPrograms->createUniformBuffer("mvp_camera");
    _shaderPrograms->createUniformBuffer("viewPosBlock");
    _shaderPrograms->createUniformBuffer("shadowSettingsBlock");
    _shaderPrograms->createUniformBuffer("viewPortBlock");

    std::tuple<glm::vec2> viewPortSize = {
        glm::vec2(_viewportWidth, _viewportHeight)
    };

    _shaderPrograms->getUniformBuffer("viewPortBlock").update(viewPortSize);

    // Initialize LightManager
    _lightLibrary = std::make_shared<LightLibrary>(this);

    // Initialize Instancing Manager
    _instancingManager = std::make_shared<InstancingManager>();

    // Initialize Rendering strategy
    switch (strategy)
    {
        case E_RenderStrategy::DeferredShading:
            _strategyChain = std::make_shared<DeferredShadingStrategyChain>(this);
            break;
        case E_RenderStrategy::PBSShading:
            _strategyChain = std::make_shared<PBSShadingStrategyChain>(this);
            break;
        default:
        case E_RenderStrategy::ForwardShading:
            _strategyChain = std::make_shared<ForwardShadingStrategyChain>(this);
            break;
    }

    return 1;
}

// Next step is to encapsulate this in a method that also handles Framebuffer changes
void openGL::renderFrame(std::shared_ptr<Scene> scene)
{
    _strategyChain->run();
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

void openGL::resizeWindow(GLFWwindow* window, int width, int height)
{
    _viewportWidth = width;
    _viewportHeight = height;
    glViewport(0, 0, _viewportWidth, _viewportHeight);

    std::tuple<glm::vec2> viewPortSize = {
        glm::vec2(_viewportWidth, _viewportHeight)
    };

    _shaderPrograms->getUniformBuffer("viewPortBlock").update(viewPortSize);

    for(auto scene : _scenes)
    {
        if (scene->getActiveCamera() != nullptr)
        {
            scene->getActiveCamera()->resizeCameraPlane((float)width, (float)height);
        }
    }

    // Resize all FBOs
    _frameBuffers->resizeViewPortBoundFBOs();

    glViewport(0, 0, _viewportWidth, _viewportHeight);

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
    // vertex tangent info
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));

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

void openGL::initializeTextureHDR(TextureHDR& texture)
{
    // load and create a texture
    glGenTextures(1, &texture._id);
    glBindTexture(GL_TEXTURE_2D, texture._id);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    // load image, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texture._width, texture._height, 0, texture._colorChannels, GL_FLOAT, texture._pixels);
}

void openGL::bindTextures(std::shared_ptr<Mesh> mesh)
{

    _shaderPrograms->setUniformInt("sampleFromDiffuse", 0);
    _shaderPrograms->setUniformInt("sampleFromSpecular", 0);
    _shaderPrograms->setUniformInt("sampleFromNormal", 0);
    _shaderPrograms->setUniformInt("sampleFromHeight", 0);

    _shaderPrograms->setUniformFloat("material.shininess", 4.5f);

    if (mesh->_textures.size() == 0)
    {
        return;
    }

    for (int i = 0; i < mesh->_textures.size(); i++)
    {

        switch (mesh->_textures[i]._type)
        {
        case DIFFUSE:
            _shaderPrograms->setUniformInt("sampleFromDiffuse", 1);
            _shaderPrograms->setUniformInt("material.diffuse", 1);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case SPECULAR:
            _shaderPrograms->setUniformInt("sampleFromSpecular", 1);
            _shaderPrograms->setUniformInt("material.specular", 2);
            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case NORMAL:
            if(_settings->getNormalMapping() == E_Setting::ON)
            {
                _shaderPrograms->setUniformInt("sampleFromNormal", 1);
                _shaderPrograms->setUniformInt("material.normal", 3);
                glActiveTexture(GL_TEXTURE0 + 3);
                glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            }
            break;
        case HEIGHT:
            if(_settings->getHeightMapping() == E_Setting::ON)
            {
                _shaderPrograms->setUniformInt("sampleFromHeight", 1);
                _shaderPrograms->setUniformInt("material.height", 4);
                glActiveTexture(GL_TEXTURE0 + 4);
                glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            }
            break;
        case ROUGHNESS:
            _shaderPrograms->setUniformInt("sampleFromRoughness", 1);
            _shaderPrograms->setUniformInt("material.roughness", 5);
            glActiveTexture(GL_TEXTURE0 + 5);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case LIGHTMAP:
            _shaderPrograms->setUniformInt("sampleFromLightmap", 1);
            _shaderPrograms->setUniformInt("material.lightmap", 6);
            glActiveTexture(GL_TEXTURE0 + 6);
            glBindTexture(GL_TEXTURE_2D, mesh->_textures[i]._id);
            break;
        case CUBEMAP:
            _shaderPrograms->setUniformInt("cubemap", 5);
            glActiveTexture(GL_TEXTURE0 + 5);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mesh->_textures[i]._id);
            break;
        default:
            break;
        }
    }
}


void openGL::initializeSkybox(Skybox &skybox, const std::array<Texture, 6>& textures)
{
    Cubemap& cubemap = *skybox.getCubemap();
    // VAO and VBO
    glGenVertexArrays(1, &cubemap.VAO);
    glGenBuffers(1, &cubemap.VBO);
    glBindVertexArray(cubemap.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubemap.VBO);

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

    glDepthFunc(GL_LEQUAL);

    _shaderPrograms->setUniformInt("skyboxCube", 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->getTexture()._id);

    glBindVertexArray(cubemap->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glActiveTexture(GL_TEXTURE0);
    glDepthFunc(GL_LESS);
}

void openGL::initializeInstanceManager(std::shared_ptr<Scene> scene)
{
    _instancingManager->setupInstancing(_shaderPrograms->getShader("Basic")->getProgramId(), _scenes[0]);
}


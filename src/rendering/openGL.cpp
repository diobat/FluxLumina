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
        static_cast<openGL*>(glfwGetWindowUserPointer(window))->resizeWindowCallback(window, width, height);
    };
    glfwSetWindowSizeCallback(_window, func);
    // End window resize code

    // Settings module initialization
    _settings = std::make_shared<Settings>(_window);

    // Mesh Library initialization
    _meshLibrary = std::make_shared<MeshLibrary>();

    // Texture Library initialization
    _textureLibrary = std::make_shared<TextureLibrary>(this);

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

void openGL::resizeWindowCallback(GLFWwindow* window, int width, int height)
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

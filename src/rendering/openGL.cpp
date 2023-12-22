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

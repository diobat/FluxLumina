#include "FluxLumina.hpp"
#include "rendering/openGLContext.hpp"

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

// First-party includes
#include <scene/Scene.hpp>

#include "scene/SceneObjectFactory.hpp"
#include "rendering/strategy/StrategyChain.hpp"
#include "rendering/shader/ShaderLibrary.hpp"
#include "user_input/glfwUserInputScanner.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"
#include "rendering/Settings.hpp"
#include "rendering/MeshLibrary.hpp"
#include "rendering/libraries/TextureLibrary.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"


FluxLumina::FluxLumina( E_RenderStrategy strategy)
{
    initialize(strategy);
}

int FluxLumina::initialize(E_RenderStrategy strategy)
{
    _scenes.clear();
    _scenes.emplace_back(std::make_shared<Scene>());
    //_scenes.push_back(scene);

    _window = InitializeOpenGLContext();

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
        static_cast<FluxLumina*>(glfwGetWindowUserPointer(window))->resizeWindowCallback(window, width, height);
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
            _strategyChain = std::make_shared<ForwardShadingStrategyChain>(this, _shaderPrograms->getShader("Basic"), _shaderPrograms->getShader("transparency"));
            break;
    }

    // Initialize User Input
    _userInput = std::make_shared<glfwKeyboardScanner>(_window);
    _userInput->bindToScene(_scenes[0]);

    // SceneObjectFactory initialization
    _sceneObjectFactory = std::make_shared<SceneObjectFactory>(_scenes[0].get(), this);

    return 1;
}

// Next step is to encapsulate this in a method that also handles Framebuffer changes
void FluxLumina::renderFrame(std::shared_ptr<Scene> scene)
{
    _strategyChain->run();
}

void FluxLumina::resizeWindowCallback(GLFWwindow* window, int width, int height)
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

void FluxLumina::update()
{
    float startTime = static_cast<float>(glfwGetTime());
    float newTime  = 0.0f;
    float gameTime = 0.0f;
    float deltaTime = 0.0f;
    std::string windowTitle;

    while (!glfwWindowShouldClose(_window))
    {
        /* Update game time value */
        newTime  = static_cast<float>(glfwGetTime());
        deltaTime = newTime - gameTime - startTime;
        gameTime = newTime - startTime;

        openGLContext::updateFPSCounter(deltaTime);

        renderFrame(_scenes[0]);
       
        /* Swap front and back buffers */
        glfwSwapBuffers(_window);
        /* Poll for and process events */
        glfwPollEvents();

        _userInput->tickCallback();
    }

    glfwTerminate();
}

boost::uuids::uuid FluxLumina::create_Model(
    const std::string &modelPath, 
    const std::string& shader, 
    bool flipUVs, 
    std::array<std::vector<std::string>, 2> textureLocationStrings
    )
{
    TextureLocations textureLocations = {
        textureLocationStrings[0],
        textureLocationStrings[1]
    };
    return _sceneObjectFactory->create_Model(modelPath, shader, flipUVs, textureLocations).id();
}

boost::uuids::uuid FluxLumina::create_Model(
    const std::vector<std::array<float, 3>>& vertices,
    const std::vector<unsigned int>& indices, 
    const std::vector<std::array<float, 3>>& colors,
    const std::string& shader)
{

    unsigned int size = vertices.size();
    if (size != colors.size())
    {
        throw std::runtime_error("Vertex and color arrays must be of the same size");
    }

    std::vector<Vertex> vertexVector;
    for (unsigned int i(0); i < size; ++i)
    {
        vertexVector.push_back(
            Vertex({
            glm::vec3(vertices[i][0], vertices[i][1], vertices[i][2]),          // Position
            glm::vec3(0.0f, 0.0f, 0.0f),                                        // Normal
            glm::vec2(0.0f, 0.0f),                                              // TexCoords
            glm::vec3(colors[i][0], colors[i][1], colors[i][2]),                // Color
            glm::vec3(0.0f, 0.0f, 0.0f)                                         // Tangent
            })
        );
    }
    return _sceneObjectFactory->create_Model(vertexVector, indices, shader).id();
}

void FluxLumina::create_Camera(float fov, float translationSpeed, float rotationSpeed)
{
    _sceneObjectFactory->create_Camera(fov, translationSpeed, rotationSpeed);
}

boost::uuids::uuid FluxLumina::create_LightSource(unsigned int type)
{
    E_LightType light_type = static_cast<E_LightType>(type);

    return _sceneObjectFactory->create_LightSource(light_type)->id();
}

unsigned int FluxLumina::create_IBL(const std::string& path, bool flipUVs)
{
    return _sceneObjectFactory->create_IBL(path, flipUVs)->getTexture()._id;
}

void FluxLumina::create_Skybox(const std::vector<std::string>& path)
{
    _sceneObjectFactory->create_Skybox(path);
}

void FluxLumina::setPosition(boost::uuids::uuid UUID, std::array<float, 3> position)
{
    _scenes[0]->get(UUID)->setPosition(position);
}

void FluxLumina::setRotation(boost::uuids::uuid UUID, std::array<float, 3> rotation)
{
    _scenes[0]->get(UUID)->rotate(rotation[0], rotation[1], rotation[2]);
}

void FluxLumina::setScale(boost::uuids::uuid UUID, float scale)
{
    _scenes[0]->get(UUID)->setScale(scale);
}

void FluxLumina::setColor(boost::uuids::uuid UUID, std::array<float, 3> color)
{
    std::shared_ptr<LightSource> light = std::dynamic_pointer_cast<LightSource>(_scenes[0]->get(UUID));

    if (light != nullptr)
    {
        light->setColor(color);
    }   
}

void FluxLumina::setAttenuationFactors(boost::uuids::uuid UUID, std::array<float, 3> attenuationFactors)
{
    std::shared_ptr<LightSource> light = std::dynamic_pointer_cast<LightSource>(_scenes[0]->get(UUID));

    if (light != nullptr)
    {
        light->setAttenuationFactors(attenuationFactors);
    }
}

void FluxLumina::setDirection(boost::uuids::uuid UUID, std::array<float, 3> direction)
{
    std::shared_ptr<SpotLight> light = std::dynamic_pointer_cast<SpotLight>(_scenes[0]->get(UUID));

    if (light != nullptr)
    {
        light->pointAt(direction);
    }
}

void FluxLumina::setSpotlightRadius(boost::uuids::uuid UUID, float radius)
{
    std::shared_ptr<SpotLight> light = std::dynamic_pointer_cast<SpotLight>(_scenes[0]->get(UUID));

    if (light != nullptr)
    {
        light->setCutoff(radius);
    }
}
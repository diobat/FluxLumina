#include "rendering/openGL.h"

openGL::openGL()
{
    ;
}

openGL::~openGL()
{
    glfwTerminate();
}

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

int openGL::initialize()
{
    int WINDOW_WIDTH = 2048;
    int WINDOW_HEIGHT = 1536;

    /* Initialize the library */
    if (!glfwInit())
        return 0;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Espigão Simulator 3000", nullptr, nullptr);

    if (!_window)
    {
        glfwTerminate();
        return 0;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
    
    /* Initialize glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }

    /* Set the viewport */
    glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearStencil(0);

    // Window resize code
    glfwSetWindowUserPointer(_window, this);

    auto func = [](GLFWwindow* window, int width, int height)
    {
        static_cast<openGL*>(glfwGetWindowUserPointer(window))->resizeWindow(window, width, height);
    };

    glfwSetWindowSizeCallback(_window, func);
    // End window resize code

    // Framebuffer Manager initialization
    _frameBuffers = std::make_unique<FBOManager>(_window);

    // Select shader program to use
    auto shader = std::make_shared<Shader>("Basic.vert", "Basic.frag");
    _shaderPrograms.push_back(shader);
    auto shader2 = std::make_shared<Shader>("Simple.vert", "Simple.frag");
    _shaderPrograms.push_back(shader2);
    auto shader3 = std::make_shared<Shader>("Basic.vert", "transparency.frag");
    _shaderPrograms.push_back(shader3);
    auto shader4 = std::make_shared<Shader>("Quad.vert", "Quad.frag");
    _shaderPrograms.push_back(shader4);
    useShader(0);

    return 1;
}



// Next step is to encapsulate this in a method that also handles Framebuffer changes
void openGL::renderFrame(std::shared_ptr<Scene> scene)
{
    // For window resizing purposes
    _scene = scene; 

    // Bind the proper FBO
    _frameBuffers->bindProperFBOFromScene(scene);

    // Reset color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera
    cameraSetup(scene);

    // Light
    allLightsSetup(scene->getAllLights());

    // Draw models
    for(int i(0); i<_shaderPrograms.size(); i++)
    {
        useShader(i);
        // Render all the non-transparent models
        std::map<float, std::shared_ptr<ModelObject>> sortedTransparentModels;
        for (auto model : scene->getModels(_shaderPrograms[i]->getProgramId()))
        {
            if(model->getModel()->hasTransparency)
            {
                float distance = glm::length(scene->getActiveCamera()->getPosition() - conversion::toVec3(model->getPosition()));
                sortedTransparentModels[distance] = model;
            }
            else
            {
                renderModel(*model);
            }
        }
        // Render all the transparent models (from in decreasing distance to the camera)
        if(!sortedTransparentModels.empty())
        {
            for (std::map<float, std::shared_ptr<ModelObject>>::reverse_iterator it = sortedTransparentModels.rbegin(); it != sortedTransparentModels.rend(); ++it)
            {
                renderModel(*it->second);
            }
        }

    }

}

void openGL::renderModel(ModelObject &model)
{
    auto xyz_array = model.getPosition();
    glm::vec3 coordinates(xyz_array[0], xyz_array[1], xyz_array[2]);

    // Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // Apply translation to the model matrix
    modelMatrix = glm::translate(modelMatrix, coordinates);
    // Apply scale to the model matrix
    modelMatrix = glm::scale(modelMatrix, glm::vec3(model.getScale()));
    // Apply rotation to the model matrix
    glm::fquat std_quat = model.getRotation();
    modelMatrix = modelMatrix * glm::mat4_cast(std_quat);

    _shaderPrograms[currentShaderIndex]->setUniformMatrix4fv("model", modelMatrix);

    for (auto &one_mesh : model.getModel()->meshes)
    {
        bindTextures(one_mesh);

        // draw mesh
        glBindVertexArray(one_mesh.VAO);
        glDrawElements(GL_TRIANGLES, one_mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
    }
}

void openGL::cameraSetup(std::shared_ptr<Scene> scene)
{
    // Camera
    scene->getActiveCamera()->recalculateMVP();
    for (int i(0); i < _shaderPrograms.size(); i++)
    {
        _shaderPrograms[i]->setUniformMatrix4fv("view", scene->getActiveCamera()->getViewMatrix());
        _shaderPrograms[i]->setUniformMatrix4fv("projection", scene->getActiveCamera()->getProjectionMatrix());
        _shaderPrograms[i]->setUniform3fv("viewPos", scene->getActiveCamera()->getPosition());
    }
}

void openGL::allLightsSetup(const LightContents &lights)
{
    
    auto directionalLights = lights.directionalLights;
    _shaderPrograms[currentShaderIndex]->setUniform1i("numDirLights", directionalLights.size());
    for(unsigned int i(0); i<directionalLights.size(); ++i)
    {
        lightSetup(i, *directionalLights[i]);
    }

    auto pointLights = lights.pointLights;
    _shaderPrograms[currentShaderIndex]->setUniform1i("numPointLights", pointLights.size());
    for(unsigned int i(0); i<pointLights.size(); ++i)
    {
        lightSetup(i, *pointLights[i]);
    }

    auto spotLights = lights.spotLights;
    _shaderPrograms[currentShaderIndex]->setUniform1i("numSpotLights", spotLights.size());
    for(unsigned int i(0); i<spotLights.size(); ++i)
    {
        lightSetup(i, *spotLights[i]);
    }

}

void openGL::lightSetup(unsigned int lightIndex, const DirectionalLight &light)
{
    // Direction
    glm::vec3 direction = conversion::toVec3(light.getDirection());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("dirLight[" + std::to_string(lightIndex) + "].direction", direction);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("dirLight[" + std::to_string(lightIndex) + "].ambient", color);
    _shaderPrograms[currentShaderIndex]->setUniform3fv("dirLight[" + std::to_string(lightIndex) + "].diffuse", color);
    _shaderPrograms[currentShaderIndex]->setUniform3fv("dirLight[" + std::to_string(lightIndex) + "].specular", color);
}

void openGL::lightSetup(unsigned int lightIndex, const PointLight &light)
{
    // Position
    glm::vec3 position = conversion::toVec3(light.getPosition());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("pointLight[" + std::to_string(lightIndex) + "].position", position);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("pointLight[" + std::to_string(lightIndex) + "].ambient", color);
    _shaderPrograms[currentShaderIndex]->setUniform3fv("pointLight[" + std::to_string(lightIndex) + "].diffuse", color);
    _shaderPrograms[currentShaderIndex]->setUniform3fv("pointLight[" + std::to_string(lightIndex) + "].specular", color);

    // Attenuation factors
    const std::array<float, 3>& attFactors = light.getAttenuationFactors();
    _shaderPrograms[currentShaderIndex]->setUniform1f("pointLight[" + std::to_string(lightIndex) + "].constant", attFactors[0]);
    _shaderPrograms[currentShaderIndex]->setUniform1f("pointLight[" + std::to_string(lightIndex) + "].linear", attFactors[1]);
    _shaderPrograms[currentShaderIndex]->setUniform1f("pointLight[" + std::to_string(lightIndex) + "].quadratic", attFactors[2]);
}

void openGL::lightSetup(unsigned int lightIndex, const SpotLight &light)
{
    // Position
    glm::vec3 position = conversion::toVec3(light.getPosition());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("spotLight[" + std::to_string(lightIndex) + "].position", position);

    // Direction
    glm::vec3 direction = conversion::toVec3(light.getDirection());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("spotLight[" + std::to_string(lightIndex) + "].direction", direction);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    _shaderPrograms[currentShaderIndex]->setUniform3fv("spotLight[" + std::to_string(lightIndex) + "].ambient", color);
    _shaderPrograms[currentShaderIndex]->setUniform3fv("spotLight[" + std::to_string(lightIndex) + "].diffuse", color);
    _shaderPrograms[currentShaderIndex]->setUniform3fv("spotLight[" + std::to_string(lightIndex) + "].specular", color);

    // Attenuation factors
    const std::array<float, 3>& attFactors = light.getAttenuationFactors();
    _shaderPrograms[currentShaderIndex]->setUniform1f("spotLight[" + std::to_string(lightIndex) + "].constant", attFactors[0]);
    _shaderPrograms[currentShaderIndex]->setUniform1f("spotLight[" + std::to_string(lightIndex) + "].linear", attFactors[1]);
    _shaderPrograms[currentShaderIndex]->setUniform1f("spotLight[" + std::to_string(lightIndex) + "].quadratic", attFactors[2]);

    // Cutoff
    std::array<float, 2> cutoff = light.getCutoff();
    _shaderPrograms[currentShaderIndex]->setUniform1f("spotLight[" + std::to_string(lightIndex) + "].innerCutOff", glm::cos(glm::radians(cutoff[0])));
    _shaderPrograms[currentShaderIndex]->setUniform1f("spotLight[" + std::to_string(lightIndex) + "].outerCutOff", glm::cos(glm::radians(cutoff[1])));
}

void openGL::resizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (_scene->getActiveCamera() != nullptr)
    {
        _scene->getActiveCamera()->resizeCameraPlane(width, height);
    }
}

void openGL::initializeMesh(Mesh& mesh)
{

    // create buffers/arrays
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

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

void openGL::bindTextures(Mesh& mesh)
{

    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;

    _shaderPrograms[currentShaderIndex]->setUniform1i("sampleFromDiffuse", 0);
    _shaderPrograms[currentShaderIndex]->setUniform1i("sampleFromSpecular", 0);

    if (mesh.textures.size() == 0)
    {
        return;
    }

    int imageUnitSpace;

    for (int i = 0; i < mesh.textures.size(); i++)
    {

        switch (mesh.textures[i]._type)
        {
        case DIFFUSE:
            imageUnitSpace = 0;
            _shaderPrograms[currentShaderIndex]->setUniform1i("sampleFromDiffuse", 1);
            _shaderPrograms[currentShaderIndex]->setUniform1i("material.diffuse", imageUnitSpace + diffuseNr);
            glActiveTexture(GL_TEXTURE0 + imageUnitSpace + diffuseNr);
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i]._id);
            diffuseNr++;
            break;
        case SPECULAR:
            imageUnitSpace = 10;
            _shaderPrograms[currentShaderIndex]->setUniform1i("sampleFromSpecular", 1);
            _shaderPrograms[currentShaderIndex]->setUniform1i("material.specular", imageUnitSpace + specularNr);
            _shaderPrograms[currentShaderIndex]->setUniform1f("material.shininess", 0.5);
            glActiveTexture(GL_TEXTURE0 + imageUnitSpace + specularNr);
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i]._id);
            specularNr++;
            break;
        case NORMAL:
            imageUnitSpace = 20;
            break;
        case HEIGHT:
            imageUnitSpace = 30;
            break;
        default:
            imageUnitSpace = 0;
            break;
        }

    }
}

unsigned int openGL::getShaderProgramID(unsigned int shaderIndex) const
{
    return _shaderPrograms[shaderIndex]->getProgramId();
}

void openGL::useShader(unsigned int shaderIndex)
{
    currentShaderIndex = shaderIndex;
    glUseProgram(_shaderPrograms[shaderIndex]->getProgramId());
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
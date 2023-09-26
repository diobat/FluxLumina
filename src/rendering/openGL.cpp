#include "rendering/openGL.h"

openGL::openGL()
{

}

openGL::~openGL()
{
    glfwTerminate();
}

void openGL::bindScene(std::shared_ptr<Scene> scene)
{
    _scene = scene;
    _userInput->bindToScene(_scene);
}

GLFWwindow* openGL::getWindowPtr()
{
    return _window;
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

    _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello Modern GL!", nullptr, nullptr);

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

    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Window resize code
    glfwSetWindowUserPointer(_window, this);

    auto func = [](GLFWwindow* window, int width, int height)
    {
        static_cast<openGL*>(glfwGetWindowUserPointer(window))->resizeWindow(window, width, height);
    };

    glfwSetWindowSizeCallback(_window, func);
    // End window resize code

    // Select shader program to use
    Shader shader("Basic.vert", "Basic.frag");
    _shaderPrograms.push_back(shader);
    _shaderPrograms[0].apply();

    // Texture newTexture;
    // newTexture.load("res/models/alliance.png");
    // _textures.push_back(newTexture);


    return 1;
}

void openGL::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Shader
    _shaderPrograms[0].apply();

    // Camera
    _shaderPrograms[0].setUniformMatrix4fv("MVP", _scene->getActiveCamera()->recalculateMVP());

    // Textures & Models
    //_textures[0].bind();

    for (auto model : _scene->getAllModels())
    {
        renderModel(*model);
    }
}

void openGL::resizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    if (_camera != nullptr)
    {
        _camera->resizeCameraPlane(width, height);
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

void openGL::bindTextures(Mesh& mesh, E_TexureType index)
{

    _shaderPrograms[0].setUniform1i("sampleFromTexture", 0);
    for (int i = 0; i < mesh.textures.size(); i++)
    {
        _shaderPrograms[0].setUniform1i("sampleFromTexture", 1);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i]._id);
    }
}

void openGL::renderModel(ModelObject& model)
{
    auto xyz_array = model.getPosition();
    glm::vec3 coordinates(xyz_array[0], xyz_array[1], xyz_array[2]);

    // Model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    //Apply translation to the model matrix
    modelMatrix = glm::translate(modelMatrix, coordinates);             
    // Apply scale to the model matrix
    modelMatrix = glm::scale(modelMatrix, glm::vec3(model.getScale())); 
    // Apply rotation to the model matrix
    glm::fquat std_quat = model.getRotation();
    modelMatrix = modelMatrix * glm::mat4_cast(std_quat);

    _shaderPrograms[0].setUniformMatrix4fv("transform", modelMatrix);

    // Lighting
    // Ambient light
    std::array<float, 3> ambColor = _scene->getAmbientLight().getColor();
    glm::vec3 ambientColor = glm::vec3(ambColor[0], ambColor[1], ambColor[2]);
    _shaderPrograms[0].setUniform3fv("ambColor", ambientColor);

    _shaderPrograms[0].setUniform1f("ambIntensity", _scene->getAmbientLight().getIntensity());
    // Diffuse light
    std::array<float, 3> diffColor = _scene->getDiffuseLight().getColor();
    glm::vec3 diffuseColor = glm::vec3(diffColor[0], diffColor[1], diffColor[2]);
    _shaderPrograms[0].setUniform3fv("diffColor", diffuseColor);

    _shaderPrograms[0].setUniform1f("diffIntensity", _scene->getDiffuseLight().getIntensity());

    std::array<float, 3> diffDirection = _scene->getDiffuseLight().getDirection();
    glm::vec3 diffuseDirection = glm::vec3(diffDirection[0], diffDirection[1], diffDirection[2]);
    _shaderPrograms[0].setUniform3fv("diffDirection", diffuseDirection);

    for ( auto& one_mesh : model.getModel()->meshes)
    {
        bindTextures(one_mesh, DIFFUSE);

        // draw mesh
        glBindVertexArray(one_mesh.VAO);
        glDrawElements(GL_TRIANGLES, one_mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);    // Unbind texture
    }
}
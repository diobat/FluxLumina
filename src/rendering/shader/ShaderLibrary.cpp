#include "rendering/shader/ShaderLibrary.hpp"
#include "helpers/RootDir.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <stdexcept>

ShaderLibrary::ShaderLibrary(const std::string &folderName) :
    _activeShader(0),
    _shaders(_contents->_shaders),
    _contents(std::make_shared<ShaderLibraryContents>())
{
    scan(folderName);
}

ShaderLibrary::~ShaderLibrary()
{
    for(auto& shader : _shaders)
    {
        glDeleteProgram(shader->getProgramId());
    }
}

GLuint ShaderLibrary::addShader(const std::string &vertexShaderFilename,
                                 const std::string &fragmentShaderFilename,
                                 const std::string &geometryShaderFilename,
                                 const std::string &tessellationControlShaderFilename,
                                 const std::string &tessellationEvaluationShaderFilename)
{
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderFilename,
                                                              fragmentShaderFilename,
                                                              geometryShaderFilename,
                                                              tessellationControlShaderFilename,
                                                              tessellationEvaluationShaderFilename);
    _shaders.emplace_back(shader);
    return _shaders.back()->getProgramId();
}

GLuint ShaderLibrary::loadShader(const std::string &folderName)
{
    std::string vertexShaderFilename = folderName + "/vertex.glsl";
    std::string fragmentShaderFilename = folderName + "/fragment.glsl";
    std::string geometryShaderFilename = folderName + "/geometry.glsl";
    std::string tessellationControlShaderFilename = folderName + "/tessellation_control.glsl";
    std::string tessellationEvaluationShaderFilename = folderName + "/tessellation_evaluation.glsl";

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderFilename,
                                                              fragmentShaderFilename,
                                                              geometryShaderFilename,
                                                              tessellationControlShaderFilename,
                                                              tessellationEvaluationShaderFilename);

    _shaders.emplace_back(shader);
    return _shaders.back()->getProgramId();
}

std::shared_ptr<Shader> ShaderLibrary::getShader(const std::string &name)
{
    for(auto& shader : _shaders)
    {
        if(shader->getName() == name)
        {
            return shader;
        }
    }
    throw std::runtime_error("Shader with name " + name + " not found.");
}

std::shared_ptr<Shader> ShaderLibrary::getShader(unsigned int index)
{
    if(index < _shaders.size())
    {
        return _shaders[index];
    }
    else
    {
        throw std::runtime_error("Shader with index " + std::to_string(index) + " not found.");
    }
}

std::vector<std::shared_ptr<Shader>> ShaderLibrary::getShaders()
{
    return _shaders;
}

unsigned int ShaderLibrary::getShaderIndex(const std::string &name) const
{
    for(unsigned int i = 0; i < _shaders.size(); ++i)
    {
        if(_shaders[i]->getName() == name)
        {
            return i;
        }
    }
    throw std::runtime_error("Shader with name " + name + " not found.");
}

std::set<unsigned int> ShaderLibrary::getShaderIndexesPerFeature() const
{
    std::set<unsigned int> shaderIndexesPerFeature;
    for(unsigned int i = 0; i < _shaders.size(); ++i)
    {
        if(_shaders[i]->isFeatureless())
        {
            shaderIndexesPerFeature.insert(i);
        }
    }
    return shaderIndexesPerFeature;
}

std::set<unsigned int> ShaderLibrary::getShaderIndexesPerFeature(E_ShaderProgramFeatures feature) const
{
    std::set<unsigned int> shaderIndexesPerFeature;
    for(unsigned int i = 0; i < _shaders.size(); ++i)
    {
        if(_shaders[i]->isFeatureSupported(feature))
        {
            shaderIndexesPerFeature.insert(i);
        }
    }
    return shaderIndexesPerFeature;
}

unsigned int ShaderLibrary::getActiveShaderIndex() const
{
    return _activeShader;
}

size_t ShaderLibrary::size() const
{
    return _shaders.size();
}

void ShaderLibrary::use(unsigned int index)
{
    _activeShader = index;
    glUseProgram(_shaders[_activeShader]->getProgramId());
}

void ShaderLibrary::use(const std::string &name)
{
    _activeShader = getShaderIndex(name);
    glUseProgram(_shaders[_activeShader]->getProgramId());
}

void ShaderLibrary::use(const std::shared_ptr<Shader> &shader)
{
    _activeShader = getShaderIndex(shader->getName());
    glUseProgram(_shaders[_activeShader]->getProgramId());
}

unsigned int ShaderLibrary::scan(const std::string &foldername)
{
    unsigned int importedShaderPrograms = 0;

    std::string folderToScan = PROJECT_ROOT_DIR + foldername;

    if(!boost::filesystem::exists(folderToScan))
    {
        return 0;
    }
    else
    {
        importedShaderPrograms = recursiveScanFolder(folderToScan, importedShaderPrograms);
    }

    return importedShaderPrograms;
}

unsigned int ShaderLibrary::userScan(const std::string &folderName)
{
    unsigned int importedShaderPrograms = 0;

    std::string folderToScan = ROOT_DIR + folderName;

    if(!boost::filesystem::exists(folderToScan))
    {
        return 0;
    }
    else
    {
        importedShaderPrograms = recursiveScanFolder(folderToScan, importedShaderPrograms);
    }

    return importedShaderPrograms;
}

unsigned int ShaderLibrary::recursiveScanFolder(const std::string& folderName, unsigned int importedShaderPrograms)
{
    std::string shaderProgramName;

    std::string vertexShaderFilename = "";
    std::string fragmentShaderFilename = "";
    std::string geometryShaderFilename = "";
    std::string tessellationControlShaderFilename = "";
    std::string tessellationEvaluationShaderFilename = "";

    // Iterate through the entire folder content
    for(auto& item : boost::filesystem::directory_iterator(folderName))
    {
        if(boost::filesystem::is_directory(item))
        {
            importedShaderPrograms = recursiveScanFolder(item.path().string(), importedShaderPrograms);
        }

        if(boost::filesystem::is_regular_file(item))
        {
            shaderProgramName = item.path().parent_path().stem().string();

            std::string filename = item.path().stem().string();
            // This is only used in the if statement conditions below
            boost::filesystem::path extension = item.path().extension().string();

            if (extension == ".vert")
            {
                if(vertexShaderFilename != "")
                {
                    return 0; //Multiple vertex shaders found for shader program name
                }
                vertexShaderFilename = folderName + "/" + filename + ".vert";
            }
            else if (extension == ".frag")
            {
                if(fragmentShaderFilename != "")
                {
                    return 0; //Multiple fragment shaders found for shader program name
                }
                fragmentShaderFilename = folderName + "/" + filename + ".frag";
            }
            else if (extension == ".geom")
            {
                if(geometryShaderFilename != "")
                {
                    return 0; //Multiple geometry shaders found for shader program name
                }
                geometryShaderFilename = folderName + "/" + filename + ".geom";
            }
            else if (extension == ".tesc")
            {
                if(tessellationControlShaderFilename != "")
                {
                    return 0; //Multiple tessellation control shaders found for shader program name
                }
                tessellationControlShaderFilename = folderName + "/" + filename + ".tesc";
            }
            else if (extension == ".tese")
            {
                if(tessellationEvaluationShaderFilename != "")
                {
                    return 0; //Multiple tessellation evaluation shaders found for shader program name
                }
                tessellationEvaluationShaderFilename = folderName + "/" + filename + ".tese";
            }
            else
            {
                continue;
            }
        }
    }

    if(vertexShaderFilename == "" || fragmentShaderFilename == "")
    {
        return importedShaderPrograms; // No vertex or fragment shader found for shader program name
    }
    // After iteration we have all the filenames, so we can create the shader program
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexShaderFilename,
                                                            fragmentShaderFilename,
                                                            geometryShaderFilename,
                                                            tessellationControlShaderFilename,
                                                            tessellationEvaluationShaderFilename);
    shader->setName(shaderProgramName);

    _shaders.emplace_back(shader);
    ++importedShaderPrograms;

    return importedShaderPrograms;
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// SPECIALIZED UNIFORM SETTERS
///////////////////////////////////////////////////////////////////////////////////////////

//Bool

void ShaderLibrary::setUniformBool(const std::string& name, bool value)
{
    _shaders[_activeShader]->setUniform1i(name, (int)value);
}

void ShaderLibrary::setUniformBool(unsigned int index, const std::string &name, bool value)
{
    getShader(index)->setUniform1i(name, (int)value);
}

void ShaderLibrary::setUniformBool(const std::string& shader, const std::string &name, bool value)
{
    getShader(shader)->setUniform1i(name, (int)value);
}

// Int

void ShaderLibrary::setUniformInt(const std::string &name, int value)
{
    _shaders[_activeShader]->setUniform1i(name, value);
}

void ShaderLibrary::setUniformInt(unsigned int index, const std::string &name, int value)
{
    getShader(index)->setUniform1i(name, value);
}

void ShaderLibrary::setUniformInt(const std::string &shader, const std::string &name, int value)
{
    getShader(shader)->setUniform1i(name, value);
}

// Float

void ShaderLibrary::setUniformFloat(const std::string &name, float value)
{
    _shaders[_activeShader]->setUniform1f(name, value);
}

void ShaderLibrary::setUniformFloat(unsigned int index, const std::string &name, float value)
{
    getShader(index)->setUniform1f(name, value);
}

void ShaderLibrary::setUniformFloat(const std::string &shader, const std::string &name, float value)
{
    getShader(shader)->setUniform1f(name, value);
}

// Vec2

void ShaderLibrary::setUniformVec2(const std::string &name, const glm::vec2 &value)
{
    _shaders[_activeShader]->setUniform2fv(name, value);
}

void ShaderLibrary::setUniformVec2(unsigned int index, const std::string &name, const glm::vec2 &value)
{
    getShader(index)->setUniform2fv(name, value);
}

void ShaderLibrary::setUniformVec2(const std::string &shader, const std::string &name, const glm::vec2 &value)
{
    getShader(shader)->setUniform2fv(name, value);
}

void ShaderLibrary::setUniformVec2(const std::string &name, float x, float y)
{
    _shaders[_activeShader]->setUniform2fv(name, glm::vec2(x, y));
}

void ShaderLibrary::setUniformVec2(unsigned int index, const std::string &name, float x, float y)
{
    getShader(index)->setUniform2fv(name, glm::vec2(x, y));
}

void ShaderLibrary::setUniformVec2(const std::string &shader, const std::string &name, float x, float y)
{
    getShader(shader)->setUniform2fv(name, glm::vec2(x, y));
}

// Vec3

void ShaderLibrary::setUniformVec3(const std::string &name, const glm::vec3 &value)
{
    _shaders[_activeShader]->setUniform3fv(name, value);
}

void ShaderLibrary::setUniformVec3(unsigned int index, const std::string &name, const glm::vec3 &value)
{
    getShader(index)->setUniform3fv(name, value);
}

void ShaderLibrary::setUniformVec3(const std::string &shader, const std::string &name, const glm::vec3 &value)
{
    getShader(shader)->setUniform3fv(name, value);
}

void ShaderLibrary::setUniformVec3(const std::string &name, float x, float y, float z)
{
    _shaders[_activeShader]->setUniform3fv(name, glm::vec3(x, y, z));
}

void ShaderLibrary::setUniformVec3(unsigned int index, const std::string &name, float x, float y, float z)
{
    getShader(index)->setUniform3fv(name, glm::vec3(x, y, z));
}

void ShaderLibrary::setUniformVec3(const std::string &shader, const std::string &name, float x, float y, float z)
{
    getShader(shader)->setUniform3fv(name, glm::vec3(x, y, z));
}

// Vec4

void ShaderLibrary::setUniformVec4(const std::string &name, const glm::vec4 &value)
{
    _shaders[_activeShader]->setUniform4fv(name, value);
}

void ShaderLibrary::setUniformVec4(unsigned int index, const std::string &name, const glm::vec4 &value)
{
    getShader(index)->setUniform4fv(name, value);
}

void ShaderLibrary::setUniformVec4(const std::string &shader, const std::string &name, const glm::vec4 &value)
{
    getShader(shader)->setUniform4fv(name, value);
}

void ShaderLibrary::setUniformVec4(const std::string &name, float x, float y, float z, float w)
{
    _shaders[_activeShader]->setUniform4fv(name, glm::vec4(x, y, z, w));
}

void ShaderLibrary::setUniformVec4(unsigned int index, const std::string &name, float x, float y, float z, float w)
{
    getShader(index)->setUniform4fv(name, glm::vec4(x, y, z, w));
}

void ShaderLibrary::setUniformVec4(const std::string &shader, const std::string &name, float x, float y, float z, float w)
{
    getShader(shader)->setUniform4fv(name, glm::vec4(x, y, z, w));
}

// Mat 2

// void ShaderLibrary::setUniformMat2(const std::string &name, const glm::mat2 &mat)
// {
//     _shaders[_activeShader].setUniformMatrix2fv(name, mat);
// }

// void ShaderLibrary::setUniformMat2(unsigned int index, const std::string &name, const glm::mat2 &mat)
// {
//     getShader(index).setUniformMatrix2fv(name, mat);
// }

// void ShaderLibrary::setUniformMat2(const std::string &shader, const std::string &name, const glm::mat2 &mat)
// {
//     getShader(shader).setUniformMatrix2fv(name, mat);
// }

// Mat 3

void ShaderLibrary::setUniformMat3(const std::string &name, const glm::mat3 &mat)
{
    _shaders[_activeShader]->setUniformMatrix3fv(name, mat);
}

void ShaderLibrary::setUniformMat3(unsigned int index, const std::string &name, const glm::mat3 &mat)
{
    getShader(index)->setUniformMatrix3fv(name, mat);
}

void ShaderLibrary::setUniformMat3(const std::string &shader, const std::string &name, const glm::mat3 &mat)
{
    getShader(shader)->setUniformMatrix3fv(name, mat);
}

// Mat4
void ShaderLibrary::setUniformMat4(const std::string &name, const glm::mat4 &mat)
{
    _shaders[_activeShader]->setUniformMatrix4fv(name, mat);
}

void ShaderLibrary::setUniformMat4(unsigned int index, const std::string &name, const glm::mat4 &mat)
{
    getShader(index)->setUniformMatrix4fv(name, mat);
}

void ShaderLibrary::setUniformMat4(const std::string &shader, const std::string &name, const glm::mat4 &mat)
{
    getShader(shader)->setUniformMatrix4fv(name, mat);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// UNIFORM BUFFERS
///////////////////////////////////////////////////////////////////////////////////////////

UniformBuffer& ShaderLibrary::createUniformBuffer(const std::string& uniformName)
{
    std::weak_ptr<ShaderLibraryContents> shaders = _contents;
    _uniformBuffers.emplace_back(UniformBuffer(shaders, uniformName));
    UniformBuffer& buffer = _uniformBuffers.back();

    buffer.bindAllShadersToUniform(uniformName);

    return buffer;
}

UniformBuffer& ShaderLibrary::getUniformBuffer(const std::string& uniformName)
{
    for(auto& buffer : _uniformBuffers)
    {
        if(buffer.handle() == uniformName)
        {
            return buffer;
        }
    }
    throw std::runtime_error("Uniform buffer with name " + uniformName + " not found.");
}

void ShaderLibrary::deleteUniformBuffer(const std::string& uniformName)
{
    for(unsigned int i = 0; i < _uniformBuffers.size(); ++i)
    {
        if(_uniformBuffers[i].handle() == uniformName)
        {
            _uniformBuffers[i].removeBindingPoint();
            unsigned int UBO = _uniformBuffers[i].id();
            glDeleteBuffers(1, &UBO);

            _uniformBuffers.erase(_uniformBuffers.begin() + i);
            return;
        }
    }
    throw std::runtime_error("Uniform buffer with name " + uniformName + " not found.");
}
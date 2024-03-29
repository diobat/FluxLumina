#pragma once

// STD Headers
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// First-Party Headers
#include "rendering/shader/ShaderLibraryContents.hpp"
#include "rendering/shader/Shader.hpp"
#include "rendering/shader/UniformBuffer.hpp"
#include "Scene/Scene.hpp"

// Third-Party Headers
#include <glad/glad.h>
#include <boost/uuid/uuid.hpp>

class ShaderLibrary
{
public:
    ShaderLibrary(const std::string &folderName = "");
    ~ShaderLibrary();

    // Shader creation
    GLuint addShader(const std::string &vertexShaderFilename,
                      const std::string &fragmentShaderFilename,
                      const std::string &geometryShaderFilename = "",
                      const std::string &tessellationControlShaderFilename = "",
                      const std::string &tessellationEvaluationShaderFilename = "");

    GLuint loadShader(const std::string &folderName);

    // Shader management
    std::shared_ptr<Shader> getShader(unsigned int index);
    std::shared_ptr<Shader> getShader(const std::string &name);
    std::vector<std::shared_ptr<Shader>> getShaders();
    unsigned int getShaderIndex(const std::string &name) const;
    std::set<unsigned int> getShaderIndexesPerFeature() const;
    std::set<unsigned int> getShaderIndexesPerFeature(E_ShaderProgramFeatures feature) const;
    unsigned int getActiveShaderIndex() const;
    size_t size() const;

    void use(unsigned int index);
    void use(const std::string &name);
    void use(const std::shared_ptr<Shader> &shader);

    // Scan for user-supplied shaders
    unsigned int userScan(const std::string &folderName);

    // Instancing
    void setupSceneMeshes(Scene* scene);

    // Uniform setters
    UniformBuffer& createUniformBuffer(const std::string& uniformName);
    UniformBuffer& getUniformBuffer(const std::string& uniformName);
    void deleteUniformBuffer(const std::string& uniformName);

    // Specialized uniform setters
    void setUniformBool(const std::string& name, bool value);
    void setUniformBool(unsigned int index, const std::string &name, bool value);
    void setUniformBool(const std::string& shader, const std::string &name, bool value);

    void setUniformInt(const std::string &name, int value);
    void setUniformInt(unsigned int index, const std::string &name, int value);
    void setUniformInt(const std::string &shader, const std::string &name, int value);

    void setUniformFloat(const std::string &name, float value);
    void setUniformFloat(unsigned int index, const std::string &name, float value);
    void setUniformFloat(const std::string &shader, const std::string &name, float value);

    void setUniformVec2(const std::string &name, const glm::vec2 &value);
    void setUniformVec2(unsigned int index, const std::string &name, const glm::vec2 &value);
    void setUniformVec2(const std::string &shader, const std::string &name, const glm::vec2 &value);

    void setUniformVec2(const std::string &name, float x, float y);
    void setUniformVec2(unsigned int index, const std::string &name, float x, float y);
    void setUniformVec2(const std::string &shader, const std::string &name, float x, float y);

    void setUniformVec3(const std::string &name, const glm::vec3 &value);
    void setUniformVec3(unsigned int index, const std::string &name, const glm::vec3 &value);
    void setUniformVec3(const std::string &shader, const std::string &name, const glm::vec3 &value);

    void setUniformVec3(const std::string &name, float x, float y, float z);
    void setUniformVec3(unsigned int index, const std::string &name, float x, float y, float z);
    void setUniformVec3(const std::string &shader, const std::string &name, float x, float y, float z);

    void setUniformVec4(const std::string &name, const glm::vec4 &value);
    void setUniformVec4(unsigned int index, const std::string &name, const glm::vec4 &value);
    void setUniformVec4(const std::string &shader, const std::string &name, const glm::vec4 &value);

    void setUniformVec4(const std::string &name, float x, float y, float z, float w);
    void setUniformVec4(unsigned int index, const std::string &name, float x, float y, float z, float w);
    void setUniformVec4(const std::string &shader, const std::string &name, float x, float y, float z, float w);

    // void setUniformMat2(const std::string &name, const glm::mat2 &mat);
    // void setUniformMat2(unsigned int index, const std::string &name, const glm::mat2 &mat);
    // void setUniformMat2(const std::string &shader, const std::string &name, const glm::mat2 &mat);

    void setUniformMat3(const std::string &name, const glm::mat3 &mat);
    void setUniformMat3(unsigned int index, const std::string &name, const glm::mat3 &mat);
    void setUniformMat3(const std::string &shader, const std::string &name, const glm::mat3 &mat);

    void setUniformMat4(const std::string &name, const glm::mat4 &mat);
    void setUniformMat4(unsigned int index, const std::string &name, const glm::mat4 &mat);
    void setUniformMat4(const std::string &shader, const std::string &name, const glm::mat4 &mat);

private:
    // Scan for included shaders
    unsigned int scan(const std::string &folderName);
    // Filetree Scanning
    unsigned int recursiveScanFolder(const std::string &folderName, unsigned int importedShadersCount = 0);
    // Shaders
    std::shared_ptr<ShaderLibraryContents> _contents;
    std::vector<std::shared_ptr<Shader>>& _shaders;
    unsigned int _activeShader;

    // Uniform Buffers
    std::vector<UniformBuffer> _uniformBuffers;

};
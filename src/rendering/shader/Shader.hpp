#pragma once

// OpenGL Headers
#include <glad/glad.h>
#include <glm/glm.hpp>

// STD Headers
#include <map>
#include <string>
#include <set>

class Shader
{
public:
    Shader(const std::string & vertexShaderFilename,
           const std::string & fragmentShaderFilename,
           const std::string & geometryShaderFilename               = "",
           const std::string & tessellationControlShaderFilename    = "",
           const std::string & tessellationEvaluationShaderFilename = "");

    virtual ~Shader();

    void setUniform1f       (const std::string & uniformName, float value);
    void setUniform1i       (const std::string & uniformName, int value);
    void setUniform1ui      (const std::string & uniformName, unsigned int value);
    void setUniform1fv      (const std::string & uniformName, GLsizei count, float * value);
    void setUniform1iv      (const std::string & uniformName, GLsizei count, int * value);
    void setUniform2fv      (const std::string & uniformName, const glm::vec2 & vector);
    void setUniform3fv      (const std::string & uniformName, const glm::vec3 & vector);
    void setUniform4fv      (const std::string & uniformName, const glm::vec4 & vector);
    void setUniformMatrix2fv(const std::string & uniformName, const glm::mat2 & matrix);
    void setUniformMatrix3fv(const std::string & uniformName, const glm::mat3 & matrix);
    void setUniformMatrix4fv(const std::string & uniformName, const glm::mat4 & matrix);
    
    GLuint getProgramId() const;
    const std::string& getName() const;
    void setName(const std::string& name);

    bool verbose = false;

private:
    std::string _name;
    std::map<std::string, GLint> uniformsLocations;

    GLuint program_id;
    bool isLinked;

    bool link();
    bool getUniformLocation(const std::string & uniform_name);
    std::string loadFile(const std::string & filename);


};


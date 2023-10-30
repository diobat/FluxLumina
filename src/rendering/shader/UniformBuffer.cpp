#include "rendering/shader/UniformBuffer.hpp"

#include <set>

namespace 
{
    std::set<unsigned int> _BINDING_POINT_IDS_IN_USE;
}

UniformBuffer::UniformBuffer(std::weak_ptr<ShaderLibraryContents> shaders, const std::string& name)  :
    _shaderLibraryContents(shaders),
    _bindingPoint(0),
    _shaderUniformHandle(name),
    _shaderUniformSize(0)
{
    glGenBuffers(1, &_UBO);
    selectBindingPoint();
}

unsigned int UniformBuffer::id() const
{
    return _UBO;
}

const std::string &UniformBuffer::handle() const
{
    return _shaderUniformHandle;
}

void UniformBuffer::removeBindingPoint()
{
    _BINDING_POINT_IDS_IN_USE.erase(_bindingPoint);
}

void UniformBuffer::bindShaderToUniform(unsigned int shader, const std::string &uniformName)
{
    unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader, uniformName.c_str());
    glUniformBlockBinding(shader, uniformBlockIndex, _bindingPoint);
}

void UniformBuffer::bindAllShadersToUniform(const std::string &uniformName)
{
    for (auto shader : _shaderLibraryContents.lock()->getShaders())
    {
        // We only care for the shaders that have the uniform in question
        if (glGetProgramResourceIndex(shader->getProgramId(), GL_UNIFORM_BLOCK, uniformName.c_str()) != GL_INVALID_INDEX ||
            glGetProgramResourceIndex(shader->getProgramId(), GL_UNIFORM, uniformName.c_str()) != GL_INVALID_INDEX)
        {
            bindShaderToUniform(shader->getProgramId(), uniformName);
        }
    }
}


unsigned int UniformBuffer::assignOpenGLBuffer(unsigned int byteSize)
{
    glBindBuffer(GL_UNIFORM_BUFFER, _UBO);
    glBufferData(GL_UNIFORM_BUFFER, byteSize, NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _UBO, 0, byteSize);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    return _UBO;
}

unsigned int UniformBuffer::selectBindingPoint()
{
    for (unsigned int i = 1; i < GL_MAX_UNIFORM_BUFFER_BINDINGS; ++i)
    {
        if (_BINDING_POINT_IDS_IN_USE.find(i) == _BINDING_POINT_IDS_IN_USE.end())
        {
            _bindingPoint = i;
            _BINDING_POINT_IDS_IN_USE.insert(i);
            break;
        }
    }
    return _bindingPoint;
}

                #include "rendering/shader/ShaderStorageBuffer.hpp"
#include "rendering/shader/ShaderLibrary.hpp"

#include <set>
#include <boost/uuid/uuid_generators.hpp>

// GLFW include
#include "rendering/GLFW_Wrapper.hpp"

namespace
{
    std::set<unsigned int> _BINDING_POINT_IDS_IN_USE;
}

ShaderStorageBufferBase::ShaderStorageBufferBase(const std::string& name, ShaderLibrary* shaderLibrary) :
    _shaderLibrary(shaderLibrary),
    _shaderBlockHandle(name),
    _uuid(boost::uuids::random_generator()())
{
    selectBindingPoint();
}

void ShaderStorageBufferBase::bindShadertoStorage(unsigned int shaderName, const std::string& layoutName)
{
    unsigned int uniformBlockIndex = glGetProgramResourceIndex(shaderName, GL_SHADER_STORAGE_BLOCK, layoutName.c_str());

    GLenum props[1] = {GL_BUFFER_BINDING};
    GLint bindingPoint[1];
    glGetProgramResourceiv(shaderName, GL_SHADER_STORAGE_BLOCK, uniformBlockIndex, 1, props, 1, NULL, bindingPoint);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint[0], _SSBO);
}

void ShaderStorageBufferBase::bindAllComputeShaders(const std::string& layoutName)
{
    for (auto shader : _shaderLibrary->getShaders())
    {
        // We only care for the shaders that have the uniform in question
        if (glGetProgramResourceIndex(shader->getProgramId(), GL_SHADER_STORAGE_BLOCK, layoutName.c_str()) != GL_INVALID_INDEX)
        {
            bindShadertoStorage(shader->getProgramId(), layoutName);
        }
    }
}

void ShaderStorageBufferBase::selectBindingPoint()
{
    for ( int i = 0; i <  GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS; ++i)
    {
        if (_BINDING_POINT_IDS_IN_USE.find(i) == _BINDING_POINT_IDS_IN_USE.end())
        {
            _bindingPoint = i;
            _BINDING_POINT_IDS_IN_USE.insert(_bindingPoint);
            break;
        }
    }
}

void ShaderStorageBufferBase::removeBindingPoint()
{
    _BINDING_POINT_IDS_IN_USE.erase(_bindingPoint);
}

unsigned int ShaderStorageBufferBase::id() const
{
    return _SSBO;
}

boost::uuids::uuid ShaderStorageBufferBase::uuid() const
{
    return _uuid;
}

const std::string& ShaderStorageBufferBase::handle() const
{
    return _shaderBlockHandle;
}

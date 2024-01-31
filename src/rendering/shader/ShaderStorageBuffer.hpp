#pragma once

#include <vector>
#include <string>
#include <memory>
#include <boost/uuid/uuid.hpp>

class ShaderLibrary;

/*
* This class is a base class for all shader storage buffers. It is not meant to be used directly.
* The template class ShaderStorageBuffer is meant to be used instead. 
* The reason for this design is that templated classes cannot be instanced at compile time, and thus
* cannot be owned by the ShaderLibrary class.
*/

class ShaderStorageBufferBase
{
public:
    ShaderStorageBufferBase(const std::string& name, ShaderLibrary* shaderLibrary);

    void bindShadertoStorage(unsigned int shaderName, const std::string& layoutName);
    void bindAllComputeShaders(const std::string& layoutName);
    void selectBindingPoint();
    void removeBindingPoint();

    unsigned int id() const;
    boost::uuids::uuid uuid() const;
    const std::string& handle() const;

protected:

    boost::uuids::uuid _uuid;       // Unique identifier for this buffer
    unsigned int _bindingPoint;     // Binding point for this buffer
    std::string _shaderBlockHandle; // Name of the handle inside the shader
    unsigned int _SSBO;             // ID of the SSB object

    ShaderLibrary* _shaderLibrary;
};


template <typename T>
class ShaderStorageBuffer : public ShaderStorageBufferBase
{
public:
    ShaderStorageBuffer(ShaderLibrary* shaderLibrary, const std::string& name, unsigned int size)
        : ShaderStorageBufferBase(name, shaderLibrary),
        _size(size)
    {
        _data.resize(size);

        glGenBuffers(1, &_SSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, _size * sizeof(T), &_data[0], GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingPoint, _SSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    
    const std::vector<T>& data() 
    {   
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, _SSBO);
        T* ptr = (T*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
        for(int i = 0; i < _size; ++i)
        {
            _data[i] = ptr[i];
        }
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        return _data; 
    }
private:
    unsigned int _size;             // Size of the _data buffer in elements
    std::vector<T> _data;
};

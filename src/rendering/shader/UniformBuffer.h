#pragma once

// GLFW include
#include "rendering/GLFW_Wrapper.h"

// Debug Glm includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// STD library includes

#include <tuple>
#include <type_traits>
#include <algorithm>

// First party includes
#include "util/Arithmetic.h"
#include "rendering/shader/ShaderLibraryContents.h"


namespace
{
    bool validityFlag = false;
    std::vector<unsigned int> std140MemoryLayout;

    // Which types are allowed in the uniform struct?
    template <typename U>
    bool verifyTypes(U obj)
    {
        if(
            typeid(obj) == typeid(bool)         || 
            typeid(obj) == typeid(int)          ||
            typeid(obj) == typeid(float)        ||
            typeid(obj) == typeid(glm::vec2)    ||
            typeid(obj) == typeid(glm::vec3)    ||
            typeid(obj) == typeid(glm::vec4)    ||
            typeid(obj) == typeid(glm::mat2)    ||
            typeid(obj) == typeid(glm::mat3)    ||
            typeid(obj) == typeid(glm::mat4)
        )
        {
            return true;
        }
        return false;
    }

    template <typename U>
    bool verifyTypes()
    {
        U obj;
        if(
            typeid(obj) == typeid(bool)         || 
            typeid(obj) == typeid(int)          ||
            typeid(obj) == typeid(float)        ||
            typeid(obj) == typeid(glm::vec2)    ||
            typeid(obj) == typeid(glm::vec3)    ||
            typeid(obj) == typeid(glm::vec4)    ||
            typeid(obj) == typeid(glm::mat2)    ||
            typeid(obj) == typeid(glm::mat3)    ||
            typeid(obj) == typeid(glm::mat4)
        )
        {
            return true;
        }
        return false;
    }

    // Return the base alignment of a type
    template <typename P>
    unsigned int baseAlignment(P)
    {
        // Can I interest you in a switch statement?
        if(typeid(P) == typeid(bool))
        {
            return 4;
        }
        else if(typeid(P) == typeid(int))
        {
            return 4;
        }
        else if(typeid(P) == typeid(float))
        {
            return 4;
        }
        else if(typeid(P) == typeid(glm::vec2))
        {
            return 8;
        }
        else if(typeid(P) == typeid(glm::vec3))
        {
            return 16;
        }
        else if(typeid(P) == typeid(glm::vec4))
        {
            return 16;  
        }
        else if(typeid(P) == typeid(glm::mat2))
        {
            return 32;  // 16 * 2
        }
        else if(typeid(P) == typeid(glm::mat3))
        {
            return 48;  // 16 * 3
        }
        else if(typeid(P) == typeid(glm::mat4))
        {
            return 64;  // 16 * 4
        }
        else
        {
            return 0;
        }
    }

    // We must also cover for array types
    template <typename T>
    bool isArray(const T& obj)
    {
        if constexpr (std::is_array_v<T>) // Being an array and having valid types are both required
        {
            if (verifyTypes<std::remove_extent<T>::type>())
            {
                return true;
            }
        }
    return false;
    }

    // Returns number of elements in array of unknown type
    template <typename T>
    unsigned int arraySize(const T& obj)
    {
        if constexpr (std::is_array_v<T>)
        {
            return std::extent<T>::value;
        }
        else
        {
            return 0;
        }
    }

    // Base template to handle an individual tuple element
    template <size_t Index, typename... Types>
    unsigned int calculateBufferSize(unsigned int size, const std::tuple<Types...> &)
    {
        // Base case: Do nothing when the index is equal to the tuple size
        if constexpr (Index < sizeof...(Types))
        {
            // Check if the current tuple element is a valid type
            if 
            (
                !(
                    verifyTypes(std::get<Index>(std::tuple<Types...>()))    ||
                    isArray(std::get<Index>(std::tuple<Types...>()))
                )        
            )
            {
                validityFlag = false;
                return 0;
            }

            // Calculate the alignment and size of the current tuple element
            unsigned int alignment;
            unsigned int sizeThis = sizeof(std::get<Index>(std::tuple<Types...>()));
            sizeThis = std::max(4u, sizeThis);  // Minimum size is 4 bytes and bools are less than that
            if (isArray(std::get<Index>(std::tuple<Types...>())))
            {
                unsigned int arrayLength = arraySize(std::get<Index>(std::tuple<Types...>()));

                alignment = 16;
                sizeThis = 16 * arrayLength;
            }
            else
            {
                alignment = baseAlignment(std::get<Index>(std::tuple<Types...>()));
            }

            // Calculate the aligned offset of the current tuple element
            size = Math::nextHighestMultiple(size, std::min(alignment,16u));

            // Add the offset to the memory layout
            std140MemoryLayout.push_back(size);

            // Add total size of the element
            size += sizeThis;

            // Recurse to the next tuple element
            size = calculateBufferSize<Index + 1>(size, std::tuple<Types...>());
        }
        // Return the size of the tuple
        return size;
    }

    template <size_t Index, typename... Types>
    void updateBufferData(const std::tuple<Types...> &data, unsigned int UBO, const std::vector<unsigned int> &memLayout)
    {
        if constexpr (Index < sizeof...(Types))
        {
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferSubData(GL_UNIFORM_BUFFER, memLayout[Index], sizeof(std::get<Index>(data)), &std::get<Index>(data));
            updateBufferData<Index + 1>(data, UBO, memLayout);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

class UniformBuffer
{
    friend class ShaderLibrary;
public:
    UniformBuffer(std::weak_ptr<ShaderLibraryContents> shaders, const std::string& name);

    unsigned int id() const;
    const std::string& handle() const;

    void bindShaderToUniform(unsigned int shader, const std::string &uniformName);
    void bindAllShadersToUniform(const std::string &uniformName);

    template <typename... Types>
    void update(const std::tuple<Types...> &object)
    {
        if(!_shaderUniformSize)
        {
            createBuffer(object);
        }
        updateBufferData<0>(object, _UBO, _memoryLayout);
    }

private:
    template <typename... Types>
    unsigned int createBuffer(const std::tuple<Types...> &object)
    {
        _memoryLayout.clear();

        _shaderUniformSize = calculateUniformBufferSize(object);

        assignOpenGLBuffer(_shaderUniformSize);

        return _UBO;
    }

    template <typename... Types>
    unsigned int calculateUniformBufferSize(const std::tuple<Types...> &object)
    {
        std140MemoryLayout.clear();
        validityFlag = true;
        unsigned int size = calculateBufferSize<0>(0, object);
        if (!validityFlag)
        {
            return 0;
        }

        _memoryLayout = std140MemoryLayout;
        return size;
    }

    unsigned int assignOpenGLBuffer(unsigned int byteSize);
    unsigned int selectBindingPoint();
    void removeBindingPoint();
    void updateBuffer(void* data);

    unsigned int _UBO;
    unsigned int _bindingPoint;
    std::string _shaderUniformHandle;
    unsigned int _shaderUniformSize;
    std::vector<unsigned int> _memoryLayout;
    std::weak_ptr<ShaderLibraryContents> _shaderLibraryContents;
};
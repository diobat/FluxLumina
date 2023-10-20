#include "rendering/shader/UniformBuffer.h"




UniformBuffer::UniformBuffer()
{

    
    // glGenBuffers(1, &_UBO);
    // glBindBuffer(GL_UNIFORM_BUFFER, _UBO);
    // glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferTemplate), NULL, GL_STATIC_DRAW);
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &_UBO);
}



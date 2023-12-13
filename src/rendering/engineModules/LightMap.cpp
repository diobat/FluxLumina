#include "rendering/engineModules/LightMap.hpp"
#include "rendering/engineModules/LightManager.hpp"

#include "rendering/GraphicalEngine.hpp"
#include "util/VertexShapes.hpp"

namespace
{

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = 
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

}

LightMap::LightMap(LightLibrary* library)
    : _library(library)
{

}

void LightMap::init(unsigned int size, unsigned int scale)
{
    // This function should only be called once as for the moment we only support 1 lightmap per scene
    if (_lightMapFBO != nullptr)
    {
        return;
    }

    // Create the lightmap cubemap from the IBL texture
    std::shared_ptr<FBOManager> framebufferManager = _library->engine()->getFBOManager();
    _size = size;

    // Check that the scale is a power of two
    if (scale == 0 || (scale & (scale - 1)) != 0)
    {
        _scale = 4u;
    }
    else
    {
        _scale = scale;
    }

    // Create a new Framebuffer to render the Lightmap into
    _lightMapFBO = framebufferManager->addFBO(E_AttachmentTemplate::LIGHTMAP,_size,_size);
    _lightMapFBO->reset(); 
    _lightMapFBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGB16F);

    // Create a new Framebuffer to render the Irradiance map into
    _irradianceFBO = framebufferManager->addFBO(E_AttachmentTemplate::LIGHTMAP,_size/_scale,_size/_scale);
    _irradianceFBO->reset();
    _irradianceFBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGB16F);
}

unsigned int LightMap::bakeFromTexture(std::shared_ptr<TextureHDR> texture)
{
    std::shared_ptr<TextureHDR> lightmap = _library->engine()->getScene()->getSkybox().getIBLmap();
    std::shared_ptr<FBOManager> framebufferManager = _library->engine()->getFBOManager();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _library->engine()->getShaderLibrary();

    framebufferManager->bindFBO(_lightMapFBO);

    shaderPrograms->use("Flat2Cube");
    shaderPrograms->setUniformInt("textureMap", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texture->_id);

    shaderPrograms->setUniformMat4("projectionMatrix", captureProjection);

    glViewport(0,0,_size,_size);
    glBindVertexArray(shapes::cube::VAO());

    for(int i(0); i < 6 ; ++i)
    {
        shaderPrograms->setUniformMat4("viewMatrix", captureViews[i]);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _lightMapFBO->getColorAttachmentID(0), 0);
        framebufferManager->clearAll();

        glDrawArrays(GL_TRIANGLES, 0, 36);  
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
    framebufferManager->unbindFBO();

    // Rendering lightmaps changed the viewport size, so we reset it
    std::array<int, 2> viewportSize = _library->engine()->getViewportSize();
    glViewport(0, 0, viewportSize[0], viewportSize[1]);

    return _lightMapFBO->getColorAttachmentID(0);
}

unsigned int LightMap::convoluteLightMap()
{
    std::shared_ptr<FBOManager> framebufferManager = _library->engine()->getFBOManager();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _library->engine()->getShaderLibrary();

    framebufferManager->bindFBO(_irradianceFBO);

    shaderPrograms->use("Cubemap_Convolution");
    shaderPrograms->setUniformInt("inputCubemap", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _lightMapFBO->getColorAttachmentID(0));
    
    shaderPrograms->setUniformMat4("projectionMatrix", captureProjection);
    unsigned int realSize = _size/_scale;

    glViewport(0,0,_size/_scale,_size/_scale);
    glBindVertexArray(shapes::cube::VAO());

    for(int i(0); i < 6 ; ++i)
    {
        shaderPrograms->setUniformMat4("viewMatrix", captureViews[i]);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _irradianceFBO->getColorAttachmentID(0), 0);
        framebufferManager->clearAll();

        glDrawArrays(GL_TRIANGLES, 0, 36);  
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
    framebufferManager->unbindFBO();

    // Rendering irradiance maps changed the viewport size, so we reset it
    std::array<int, 2> viewportSize = _library->engine()->getViewportSize();
    glViewport(0, 0, viewportSize[0], viewportSize[1]);

    return _irradianceFBO->getColorAttachmentID(0);
}

std::shared_ptr<FBO> LightMap::getLightMapFBO() const
{
    return _lightMapFBO;
}

std::shared_ptr<FBO> LightMap::getIrradianceFBO() const
{
    return _irradianceFBO;
}

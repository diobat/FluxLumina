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

void LightMap::init()
{
    // This function should only be called once as for the moment we only support 1 lightmap per scene
    if (_lightMapFBO != nullptr)
    {
        return;
    }

    // Create the lightmap cubemap from the IBL texture
    std::shared_ptr<FBOManager> framebufferManager = _library->engine()->getFBOManager();

    // Create a new Framebuffer to render the Lightmap into
    _lightMapFBO = framebufferManager->addFBO(E_AttachmentTemplate::LIGHTMAP,512,512);
    _lightMapFBO->reset(); 
    _lightMapFBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGB16F);
    //_lightMapFBO->addAttachment(E_AttachmentSlot::DEPTH);
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

    glViewport(0,0,512,512);
    glBindVertexArray(shapes::cube::VAO());

    unsigned int colorID = _lightMapFBO->getColorAttachmentID(0);

    for(int i(0); i < 6 ; ++i)
    {
        shaderPrograms->setUniformMat4("viewMatrix", captureViews[i]);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, _lightMapFBO->getColorAttachmentID(0), 0);
        framebufferManager->clearAll();

        glDrawArrays(GL_TRIANGLES, 0, 36);  
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    // Rendering lightmaps changed the viewport size, so we reset it
    std::array<int, 2> viewportSize = _library->engine()->getViewportSize();
    glViewport(0, 0, viewportSize[0], viewportSize[1]);

    return _lightMapFBO->getColorAttachmentID(0);
}


#include "rendering/strategy/StrategyNode.hpp" 
#include "rendering/strategy/StrategyChain.hpp"

#include "rendering/GraphicalEngine.hpp"
#include "scene/Scene.hpp"

#include "rendering/shader/ShaderLibrary.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"
#include "rendering/Settings.hpp"

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// STRATEGY NODE (Pure abstract class)
///////////////////////////////////////////////////////////////////////////////////////////

StrategyNode::StrategyNode(const StrategyChain* chain) : 
    _chain(chain) 
{
    ;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// CAMERA NODE
///////////////////////////////////////////////////////////////////////////////////////////

void CameraSetupNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();

    // Camera
    scene->getActiveCamera()->recalculateMVP();

    std::tuple<glm::mat4, glm::mat4> mvp = {
        scene->getActiveCamera()->getViewMatrix(),
        scene->getActiveCamera()->getProjectionMatrix()
        };

    std::tuple<glm::vec3> cameraPosition = {
        scene->getActiveCamera()->getPosition()
        };

    shaderPrograms->getUniformBuffer("mvp_camera").update(mvp);
    shaderPrograms->getUniformBuffer("viewPosBlock").update(cameraPosition);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// SHADOWS NODE
///////////////////////////////////////////////////////////////////////////////////////////

void ShadowsSetupNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<LightLibrary> lightLibrary = _chain->engine()->getLightLibrary();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<Settings> settings = _chain->engine()->getSettings();

    if(settings->getShadowGlobal() != E_Setting::ON)
    {
        return;
    }

    std::tuple<bool, bool, bool> shadowSettings = {
        settings->getShadowDirectional() == E_Setting::ON,
        settings->getShadowPoint() == E_Setting::ON,
        settings->getShadowSpot() == E_Setting::ON
    };
    
    shaderPrograms->getUniformBuffer("shadowSettingsBlock").update(shadowSettings);

    shaderPrograms->use(0);
    lightLibrary->alignShadowMaps(scene);
    lightLibrary->renderShadowMaps(scene);
    
    // Rendering shadowmaps might have changed the viewport size, so we reset it
    std::array<int, 2> viewportSize = _chain->engine()->getViewportSize();
    glViewport(0, 0, viewportSize[0], viewportSize[1]);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHTS NODE
///////////////////////////////////////////////////////////////////////////////////////////

void LightsSetupNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<LightLibrary> lightLibrary = _chain->engine()->getLightLibrary();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();

    shaderPrograms->use(0);
    lightLibrary->prepare(scene->getAllLights());
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// FRAMEBUFFER NODE
///////////////////////////////////////////////////////////////////////////////////////////

void FramebufferNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    // Bind the proper FBO
    frameBuffers->bindProperFBOFromScene(scene);
    // Reset color and depth buffers 
    frameBuffers->clearAll();
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// OPAQUES RENDER NODE
///////////////////////////////////////////////////////////////////////////////////////////

void RenderOpaqueNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<InstancingManager> instancingManager = _chain->engine()->getInstancingManager();

    // Fetch relevant shaders
    std::set <unsigned int> shaderIndexes = shaderPrograms->getShaderIndexesPerFeature();
    std::set <unsigned int> shaderIndexesWithInstancing = shaderPrograms->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_AUTO_INSTANCING);

    std::set<unsigned int> shaderIndexesTotal;
    std::merge(shaderIndexes.begin(), shaderIndexes.end(), shaderIndexesWithInstancing.begin(), shaderIndexesWithInstancing.end(), std::inserter(shaderIndexesTotal, shaderIndexesTotal.begin()));

    // Opaque Models
    for(unsigned int shaderIndex : shaderIndexesTotal)
    {
        // Activate shader
        shaderPrograms->use(shaderIndex);

        // Go down the instancing path if the shader supports it
        if(shaderPrograms->getShader(shaderIndex)->isFeatureSupported(E_ShaderProgramFeatures::E_AUTO_INSTANCING))
        {
            _chain->engine()->renderInstancedMeshes(instancingManager);
        }
        else // Else just render on a per-model basis
        {
            for (auto model : scene->getModels(shaderPrograms->getShader(shaderIndex)->getProgramId()))
            {
                _chain->engine()->renderModel(*model);   
            }
        }
    }
    
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// SKYBOX RENDER NODE
///////////////////////////////////////////////////////////////////////////////////////////

void RenderSkyboxNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();

    //Draw Skybox if it exists
    if (scene->getSkybox().getCubemap() != nullptr)
    {
        auto view = glm::mat4(glm::mat3(scene->getActiveCamera()->getViewMatrix())); // remove translation from the view matrix
        auto projection = scene->getActiveCamera()->getProjectionMatrix();

        shaderPrograms->use(4);
        shaderPrograms->setUniformMat4(4, "view", view);
        shaderPrograms->setUniformMat4(4, "projection", projection);

        _chain->engine()->renderSkybox(scene->getSkybox());
    }

}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// TRANSPARENTS RENDER NODE
///////////////////////////////////////////////////////////////////////////////////////////

void RenderTransparentNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();


    for(unsigned int shaderIndex : shaderPrograms->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_TRANSPARENCY))
    {
        // Activate shader
        shaderPrograms->use(shaderIndex);
        std::map<float, std::shared_ptr<ModelObject>> sortedTransparentModels;
        
        for (auto model : scene->getModels(shaderPrograms->getShader(shaderIndex)->getProgramId()))
        {
            if(model->getModel()->hasTransparency())
            {
                float distance = glm::length(scene->getActiveCamera()->getPosition() - conversion::toVec3(model->getPosition()));
                sortedTransparentModels[distance] = model;
            }
        }
        // Render all the transparent models (from in decreasing distance to the camera)
        if(!sortedTransparentModels.empty())
        {
            for (std::map<float, std::shared_ptr<ModelObject>>::reverse_iterator it = sortedTransparentModels.rbegin(); it != sortedTransparentModels.rend(); ++it)
            {
                _chain->engine()->renderModel(*it->second);
            }
        }
    }

}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// HIGH DYNAMIC RANGE NODE
///////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
}

void HighDynamicRangeNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    frameBuffers->unbindFBO();
    frameBuffers->clearAll();
    unsigned int textureID = frameBuffers->getSceneFBO(scene)->getColorAttachmentID(0);
    std::set quadShaders = shaderPrograms->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_QUAD);

    if(quadVAO == 0)
    {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    for (auto quadShader : quadShaders)
    {
        shaderPrograms->use(quadShader);
        shaderPrograms->setUniformInt("material.diffuse", 1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
    frameBuffers->bindProperFBOFromScene(scene);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFAULT FRAMEBUFFER NODE
///////////////////////////////////////////////////////////////////////////////////////////

void DefaultFramebufferNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    std::shared_ptr<FBO> sourceFBO = frameBuffers->getSceneFBO(scene);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFBO->id());    // Source Framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);                  // Destination (default) Framebuffer

    unsigned int originWidth = sourceFBO->getOriginalSize()[0];
    unsigned int originHeight = sourceFBO->getOriginalSize()[1];

    unsigned int viewportWidth = _chain->engine()->getViewportSize()[0];
    unsigned int viewportHeight = _chain->engine()->getViewportSize()[1];

    glBlitFramebuffer(
        0, 0, 
        originWidth, originHeight,
        0, 0,
        viewportWidth, viewportHeight,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    // Remove binds
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

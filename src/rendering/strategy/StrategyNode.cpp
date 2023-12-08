#include "rendering/strategy/StrategyNode.hpp" 
#include "rendering/strategy/StrategyChain.hpp"

#include "rendering/GraphicalEngine.hpp"
#include "scene/Scene.hpp"

#include "rendering/shader/ShaderLibrary.hpp"
#include "rendering/engineModules/LightManager.hpp"
#include "rendering/engineModules/InstancingManager.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"
#include "rendering/Settings.hpp"

#include "util/VertexShapes.hpp"
#include "util/TexManip.hpp"

#include <random>
#include <cmath>

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

    shaderPrograms->use("Basic");
    lightLibrary->alignShadowMaps(scene);
    lightLibrary->renderShadowMaps(scene);
    
    // Rendering shadowmaps might have changed the viewport size, so we reset it
    std::array<int, 2> viewportSize = _chain->engine()->getViewportSize();
    glViewport(0, 0, viewportSize[0], viewportSize[1]);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHTS NODE
///////////////////////////////////////////////////////////////////////////////////////////

LightsSetupNode::LightsSetupNode(const StrategyChain* chain, const std::string& shader) : 
    StrategyNode(chain),
    _ShaderName(shader)
{
    ;
}

void LightsSetupNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<LightLibrary> lightLibrary = _chain->engine()->getLightLibrary();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();

    shaderPrograms->use(_ShaderName);
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
    // Tell OpenGL how many attachments we are using
    std::vector<unsigned int> colorAttachments;
    for (auto attachment : frameBuffers->getSceneFBO(scene)->getColorAttachments())
    {
        colorAttachments.push_back(attachment.slot);
    }
    glDrawBuffers(colorAttachments.size(), colorAttachments.data());  

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

    shaderPrograms->use("Basic");
    _chain->engine()->renderInstancedMeshes(instancingManager);


    for(auto shader : shaderPrograms->getShaders())
    {
        std::vector<std::shared_ptr<ModelObject>> objectsForThisShader = scene->getModels(shader->getName());

        if(
            objectsForThisShader.empty() || 
            shader->isFeatureSupported(E_ShaderProgramFeatures::E_AUTO_INSTANCING) ||   // Instanced Objects were already rendered
            shader->isFeatureSupported(E_ShaderProgramFeatures::E_TRANSPARENCY)         // And transparent ones will be rendered later
            )
        {
            continue;
        }

        shaderPrograms->use(shader);

        for(auto modelObject : objectsForThisShader)
        {
            if(!modelObject->enabled())
            {
                continue;
            }
            _chain->engine()->renderModel(*modelObject);
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

        shaderPrograms->use("Skybox");
        shaderPrograms->setUniformMat4("view", view);
        shaderPrograms->setUniformMat4("projection", projection);

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
        
        for (auto model : scene->getModels(shaderPrograms->getShader(shaderIndex)->getName()))
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
/////////////////////////// BLOOM NODE
///////////////////////////////////////////////////////////////////////////////////////////


BloomNode::BloomNode(const StrategyChain* chain) : 
    StrategyNode(chain) 
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    // Create the ping pong FBOs
    _pingPongFBOs[0] = frameBuffers->addFBO(E_AttachmentTemplate::TEXTURE, _chain->engine()->getViewportSize()[0], _chain->engine()->getViewportSize()[1]);
    _pingPongFBOs[0]->bindToViewportSize(true);
    _pingPongFBOs[0]->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);

    _pingPongFBOs[1] = frameBuffers->addFBO(E_AttachmentTemplate::TEXTURE, _chain->engine()->getViewportSize()[0], _chain->engine()->getViewportSize()[1]);
    _pingPongFBOs[1]->bindToViewportSize(true);
    _pingPongFBOs[1]->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);
}

BloomNode::~BloomNode()
{
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    frameBuffers->removeFBO(_pingPongFBOs[0]);
    frameBuffers->removeFBO(_pingPongFBOs[1]);
}

void BloomNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    
    // Activate proper shader program
    auto bloomShader = shaderPrograms->getShader("Bloom");

    shaderPrograms->use(bloomShader);
    shaderPrograms->setUniformInt("material.diffuse", 1);   

    // Loop control variables
    bool horizontal = true, firstIteration = true;
    unsigned int horizontal_value;
    int amount = 10;
    
    for(int i(0) ; i < amount; ++i)
    {
        horizontal_value = horizontal ? 1 : 0;
        frameBuffers->bindFBO(_pingPongFBOs[horizontal]);
        shaderPrograms->setUniformInt("horizontal", horizontal_value);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, firstIteration ? frameBuffers->getSceneFBO(scene)->getColorAttachmentID(1) : _pingPongFBOs[!horizontal]->getColorAttachmentID(0));

        glBindVertexArray(shapes::quad::VAO());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        horizontal = !horizontal;
        firstIteration = false;
    }

    // Activate proper shader program
    auto bloomBlendShaderProgram = shaderPrograms->getShader("BloomBlend");
    shaderPrograms->use(bloomBlendShaderProgram);

    frameBuffers->bindProperFBOFromScene(scene);

    shaderPrograms->setUniformInt("material.diffuse", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, frameBuffers->getSceneFBO(scene)->getColorAttachmentID(0));

    shaderPrograms->setUniformInt("material.specular", 2);   
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, _pingPongFBOs[horizontal]->getColorAttachmentID(0));

    glBindVertexArray(shapes::quad::VAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    // Remove binds
    frameBuffers->unbindFBO();
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// HIGH DYNAMIC RANGE NODE
///////////////////////////////////////////////////////////////////////////////////////////

HighDynamicRangeNode::HighDynamicRangeNode(const StrategyChain* chain) : 
    StrategyNode(chain) 
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    // Create the HDR FBO
    _HDRfbo = frameBuffers->addFBO(E_AttachmentTemplate::TEXTURE, _chain->engine()->getViewportSize()[0], _chain->engine()->getViewportSize()[1]);
    _HDRfbo->bindToViewportSize(true);
    _HDRfbo->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);
}

void HighDynamicRangeNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    TextureManipulator& texManip = TextureManipulator::Instance();

    texManip.copyTexture(
        frameBuffers->getSceneFBO(scene)->getColorAttachmentID(0),
        _HDRfbo->getColorAttachmentID(0),
        _chain->engine()->getViewportSize()[0],
        _chain->engine()->getViewportSize()[1]
    );


    frameBuffers->bindProperFBOFromScene(scene);

    unsigned int textureID = _HDRfbo->getColorAttachmentID(0);
    auto quadShader = shaderPrograms->getShader("Quad_HDR");

    shaderPrograms->use(quadShader);
    shaderPrograms->setUniformInt("material.diffuse", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(shapes::quad::VAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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
        viewportWidth, viewportHeight,
        0, 0,
        viewportWidth, viewportHeight,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    // Remove binds
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFERRED SHADING NODES
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// GEOMETRY PASS NODE
///////////////////////////////////////////////////////////////////////////////////////////

void GeometryPassNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<InstancingManager> instancingManager = _chain->engine()->getInstancingManager();

    // Activate proper shader program
    auto& geometryShaderProgram = shaderPrograms->getShader("deferred_geometry");
    shaderPrograms->use(geometryShaderProgram);

    _chain->engine()->renderInstancedMeshes(instancingManager);
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHT PASS NODE
///////////////////////////////////////////////////////////////////////////////////////////

void LightPassNode::run()
{

    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    // Activate proper shader program
    auto& lightShaderProgram = shaderPrograms->getShader("deferred_lighting");
    shaderPrograms->use(lightShaderProgram);

    // Enable the default scene FBO
    frameBuffers->bindProperFBOFromScene(scene);

    shaderPrograms->setUniformInt("gData.position", 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(1));
 
    shaderPrograms->setUniformInt("gData.normal", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(2));
 
    shaderPrograms->setUniformInt("gData.albedo", 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(3));

    shaderPrograms->setUniformInt("ssaoOcclusion", 3);
    glActiveTexture(GL_TEXTURE0 + 3);
    auto occlusionTextureID = _chain->getNode<SSAONode>()->getFBO()->getColorAttachmentID(1);
    glBindTexture(GL_TEXTURE_2D, occlusionTextureID);


    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glDepthMask(GL_FALSE);  // Prevents depth buffer writes    

    glBindVertexArray(shapes::quad::VAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);   // Re-enable depth buffer writes
    glActiveTexture(GL_TEXTURE0);        

}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHT VOLUMES NODE
///////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    void bindLightUniforms(const std::shared_ptr<LightSource> light, std::shared_ptr<ShaderLibrary> shaderPrograms)
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 position = conversion::toVec3(light->getPosition());
        model = glm::translate(model, position);
        float range = light->calculateMaxRange();
        model = glm::scale(model, range * glm::vec3(1.0f));
        shaderPrograms->setUniformMat4("model", model);
        shaderPrograms->setUniformFloat("pointLight.maxRange", range);
        shaderPrograms->setUniformVec3("pointLight.position", position);
        shaderPrograms->setUniformVec3("pointLight.diffuse", conversion::toVec3(light->getColor()));

        // Attenuation factors
        const std::array<float, 3>& attFactors = light->getAttenuationFactors();
        shaderPrograms->setUniformFloat("pointLight.constant", attFactors[0]);
        shaderPrograms->setUniformFloat("pointLight.linear", attFactors[1]);
        shaderPrograms->setUniformFloat("pointLight.quadratic", attFactors[2]);
    }
}

LightVolumeNode::LightVolumeNode(const StrategyChain* chain) : 
    StrategyNode(chain) 
{
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();
    
    _fbo = frameBuffers->addFBO(E_AttachmentTemplate::TEXTURE, _chain->engine()->getViewportSize()[0], _chain->engine()->getViewportSize()[1]);
    _fbo->bindToViewportSize(true);

    _fbo->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);
}

void LightVolumeNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();    

    // Bind the proper FBO
    frameBuffers->bindFBO(_fbo);
    frameBuffers->clearColor();

    // Activate proper shader program
    auto& lightShaderProgram = shaderPrograms->getShader("deferred_light_volumes");
    shaderPrograms->use(lightShaderProgram);

    shaderPrograms->setUniformInt("gData.position", 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(1));

    shaderPrograms->setUniformInt("gData.normal", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(2));
 
    shaderPrograms->setUniformInt("gData.albedo", 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(3));

    // Lights are additively blended
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    // Draw Light Volumes back faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glBindVertexArray(shapes::sphere::VAO());
    // Light Volumes: Draw Light Sources as geometry, calculate lighting
    const std::vector<std::shared_ptr<PointLight>>& pointLights = scene->getAllLights().pointLights;
    for(auto& pointLight : pointLights)
    {
        bindLightUniforms(pointLight, shaderPrograms);
        glDrawElements(GL_TRIANGLES, shapes::sphere::indexCount(), GL_UNSIGNED_INT, 0);
    }   

    // Return to normal settings
    glCullFace(GL_BACK);

    // Blend the light volumes with the scene
    // Re-enable the default scene FBO again
    frameBuffers->bindProperFBOFromScene(scene);
    // Switch shaders
    auto quadShader = shaderPrograms->getShader("Quad_Texture");
    shaderPrograms->use(quadShader);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);    
    shaderPrograms->setUniformInt("tex.color", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbo->getColorAttachmentID(0));

    glDepthMask(GL_FALSE);  // Prevents depth buff}er writes

    glBindVertexArray(shapes::quad::VAO());    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Return to normal settings
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);  // Prevents depth buffer writes      

}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// SSAO NODE
///////////////////////////////////////////////////////////////////////////////////////////

SSAONode::SSAONode(const StrategyChain* chain) :
    StrategyNode(chain)
{
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    // Generate the SSAO framebuffer    
    _occlusionFBO = frameBuffers->addFBO(E_AttachmentTemplate::TEXTURE, _chain->engine()->getViewportSize()[0], _chain->engine()->getViewportSize()[1]);
    _occlusionFBO->bindToViewportSize(true);
    _occlusionFBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RED);  // Pre - blur
    _occlusionFBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RED);  // Post - blur


    // Generate the kernel
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;

    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0f - 1.0f, 
            randomFloats(generator) * 2.0f - 1.0f, 
            0.0f
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);

        // // Scale samples s.t. they're more aligned to center of kernel
        // float scale = float(i) / 64.0;
        // scale = std::lerp(0.1f, 1.0f, scale * scale);
        // sample *= scale;
        _ssaoKernel.push_back(sample);
    };

    // Generate noise texture
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator) * 2.0 - 1.0, 
            0.0f); 
        _ssaoNoise.push_back(noise);
    }  

    glGenTextures(1, &_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, _noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &_ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void SSAONode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();
    std::shared_ptr<FBOManager> frameBuffers = _chain->engine()->getFBOManager();

    // Bind the proper FBO
    frameBuffers->bindFBO(_occlusionFBO);
    frameBuffers->clearAll();

    // Use the correct shader
    auto& ssaoShaderProgram = shaderPrograms->getShader("deferred_ssao");
    shaderPrograms->use(ssaoShaderProgram);

    // Update the Geometry Pass outputs to the uniform slots
    shaderPrograms->setUniformInt("gData.position", 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(4));

    shaderPrograms->setUniformInt("gData.normal", 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, _chain->engine()->getFBOManager()->getSceneFBO(scene)->getColorAttachmentID(5));
 
    shaderPrograms->setUniformInt("noiseTex", 2);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, _noiseTexture);
    glActiveTexture(GL_TEXTURE0);

    for (unsigned int i = 0; i < 64; ++i)
    {
        shaderPrograms->setUniformVec3("samples[" + std::to_string(i) + "]", _ssaoKernel[i]);
    }
 
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBindVertexArray(shapes::quad::VAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);   

    // Blur the SSAO texture
    shaderPrograms->use("deferred_SSAO_blur");

    shaderPrograms->setUniformInt("screenTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _occlusionFBO->getColorAttachmentID(0));

    glDrawBuffer(GL_COLOR_ATTACHMENT1);   
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);   

    frameBuffers->unbindFBO();
}

std::shared_ptr<FBO> SSAONode::getFBO() const
{
    return _occlusionFBO;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEBUG NODES
///////////////////////////////////////////////////////////////////////////////////////////

LightSourceCubeDebugNode::LightSourceCubeDebugNode(const StrategyChain* chain, bool depthTest) : 
    StrategyNode(chain),
    _depthTest(depthTest)
{
    ;
}

void LightSourceCubeDebugNode::run()
{
    std::shared_ptr<Scene> scene = _chain->engine()->getScene();
    std::shared_ptr<ShaderLibrary> shaderPrograms = _chain->engine()->getShaderLibrary();

    glDisable(GL_CULL_FACE);

    glDepthMask(GL_FALSE);  // Prevents depth buffer writes

    if(!_depthTest)
    {
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    shaderPrograms->use("Simple");

    const std::vector<std::shared_ptr<PointLight>>& pointLights = scene->getAllLights().pointLights;

    glBindVertexArray(shapes::sphere::VAO());

    for(const auto& pointLight : pointLights)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, conversion::toVec3(pointLight->getPosition()));
        model = glm::scale(model, pointLight->calculateMaxRange() * glm::vec3(1.0f));
        shaderPrograms->setUniformMat4("model", model);
        glm::vec4 color = glm::vec4(conversion::toVec3(pointLight->getColor()), 1.0f);
        shaderPrograms->setUniformVec4("outputColor", color);

        glDrawElements(GL_TRIANGLES, shapes::sphere::indexCount(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
    
    if(!_depthTest)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glDepthMask(GL_TRUE);  // Prevents depth buffer writes
    glEnable(GL_CULL_FACE);
}
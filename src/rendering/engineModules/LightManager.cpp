#include "rendering/engineModules/LightManager.hpp"

#include "rendering/GraphicalEngine.hpp"

#include <stdexcept>


namespace
{
    E_LightType getLightType(const LightSource& light)
    {
        if(dynamic_cast<const DirectionalLight*>(&light))
        {
            return E_LightType::DIRECTIONAL_LIGHT;
        }
        else if(dynamic_cast<const PointLight*>(&light))
        {
            return E_LightType::POINT_LIGHT;
        }
        else if(dynamic_cast<const SpotLight*>(&light))
        {
            return E_LightType::SPOT_LIGHT;
        }
        else
        {
            throw std::runtime_error("Light type not recognized");
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// SHADOWMAP
///////////////////////////////////////////////////////////////////////////////////////////


E_LightType ShadowMap::getLightType() const
{
    return _lightType;
}

void ShadowMap::alignShadowMap(std::shared_ptr<LightSource> light)
{
    if(_shadowDepthBuffer.lock() == nullptr)
    {
        throw std::runtime_error(" Depth buffer not bound");
    }

    std::shared_ptr<DirectionalLight> light_directional;
    std::shared_ptr<PointLight> light_point;
    std::shared_ptr<SpotLight> light_spot; 

    switch(_lightType)
    {
        case E_LightType::DIRECTIONAL_LIGHT:
            light_directional = std::dynamic_pointer_cast<DirectionalLight>(light);
            if(!light_directional)
            {
                throw std::runtime_error("Light type mismatch");
            }
            break;
        case E_LightType::POINT_LIGHT:
            light_point = std::dynamic_pointer_cast<PointLight>(light);
            if(!light_point)
            {
                throw std::runtime_error("Light type mismatch");
            }
            break;
        case E_LightType::SPOT_LIGHT:
            light_spot = std::dynamic_pointer_cast<SpotLight>(light);
            if(!light_spot)
            {
                throw std::runtime_error("Light type mismatch");
            }
            break;
        default:
            throw std::runtime_error("Light type not recognized");
    }

   // Calculate light space transform

    _nearPlane = 1.0f;

    glm::vec3 observed_point;
    float fov;

    glm::mat4 perMat;
    glm::vec3 posVec;

   switch(_lightType)
   {
    case E_LightType::DIRECTIONAL_LIGHT:
        break;	
    case E_LightType::POINT_LIGHT:

        _farPlane = 1000.0f;
        fov = glm::radians(90.0f);

        perMat = glm::perspective(fov, 1.0f, _nearPlane, _farPlane);
        posVec = conversion::toVec3(light_point->getPosition());

        _lightSpaceMatrix.clear();

        // Right face
        observed_point = posVec + glm::vec3( 1.0f, 0.0f, 0.0f);
        _lightSpaceMatrix.push_back(perMat*glm::lookAt(posVec,observed_point,glm::vec3(0.0f,-1.0f, 0.0f)));
        // Left face
        observed_point = posVec + glm::vec3(-1.0f, 0.0f, 0.0f);
        _lightSpaceMatrix.push_back(perMat*glm::lookAt(posVec,observed_point,glm::vec3(0.0f,-1.0f, 0.0f)));
        // Top face
        observed_point = posVec + glm::vec3( 0.0f, 1.0f, 0.0f);
        _lightSpaceMatrix.push_back(perMat*glm::lookAt(posVec,observed_point,glm::vec3(0.0f, 0.0f, 1.0f)));
        // Bottom face
        observed_point = posVec + glm::vec3( 0.0f,-1.0f, 0.0f);
        _lightSpaceMatrix.push_back(perMat*glm::lookAt(posVec,observed_point,glm::vec3(0.0f, 0.0f,-1.0f)));
        // Front face
        observed_point = posVec + glm::vec3( 0.0f, 0.0f, 1.0f);
        _lightSpaceMatrix.push_back(perMat*glm::lookAt(posVec,observed_point,glm::vec3(0.0f,-1.0f, 0.0f)));
        // Back face
        observed_point = posVec + glm::vec3( 0.0f, 0.0f,-1.0f);
        _lightSpaceMatrix.push_back(perMat*glm::lookAt(posVec,observed_point,glm::vec3(0.0f,-1.0f, 0.0f)));
        break;

    case E_LightType::SPOT_LIGHT:
        _farPlane = 1000.0f;
        fov = light_spot->getCutoff()[1]*1.25f;
        perMat = glm::perspective(fov, 1.0f, _nearPlane, _farPlane);
        posVec = conversion::toVec3(light_spot->getPosition());
        observed_point = posVec + conversion::toVec3(light_spot->getDirection());

        glm::mat4 lightSpaceMatrix = perMat * glm::lookAt(posVec,observed_point,glm::vec3(0.0f, 1.0f, 0.0f));
        _lightSpaceMatrix.push_back(lightSpaceMatrix);

        break;    
   } 
}

std::shared_ptr<FBO> ShadowMap::getShadowMap() const
{
    return _shadowDepthBuffer.lock();
}

const glm::mat4& ShadowMap::getLightSpaceMatrix(unsigned int index) const
{
    return _lightSpaceMatrix[index];
}

void ShadowMap::setLightType(E_LightType type)
{
    _lightType = type;
}

void ShadowMap::setShadowBuffer(std::shared_ptr<FBO> shadowMap)
{
    _shadowDepthBuffer = shadowMap;
}

void ShadowMap::setDimensions(unsigned int width, unsigned int height)
{
    _bufferWidth = width;
    if(height == 0)
    {
        _bufferHeight = width;
    }
    else
    {
        _bufferHeight = height;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHT LIBRARY
///////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    unsigned int getShadowResolution(GraphicalEngine* engine)
    {
        auto setting = engine->getSettings()->getShadowQualityGlobal();
        switch(setting)
        {
            case E_ShadowQuality_Global::LOW:
                return 512;
                break;
            case E_ShadowQuality_Global::MEDIUM:
                return 1024;
                break;
            case E_ShadowQuality_Global::HIGH:
                return 2048;
                break;
            case E_ShadowQuality_Global::ULTRA:
                return 4096;
                break;
            default:
                throw std::runtime_error("Shadow quality not recognized");
        }
        return 512;
    }
}

LightLibrary::LightLibrary(GraphicalEngine* engine) :
    _ranFrom(engine)
{
    ;
}

void LightLibrary::bindFramebufferManager(std::weak_ptr<FBOManager> framebufferManager)
{
    _framebufferManager = framebufferManager;
}

void LightLibrary::bindShaderLibrary(std::weak_ptr<ShaderLibrary> shaderLibrary)
{
    _shaderLibrary = shaderLibrary;
}

bool LightLibrary::prepare(const LightContents& lights)
{
    auto shaders = _shaderLibrary.lock();

    auto& directionalLights = lights.directionalLights;
    int directionalLightsCount = static_cast<int>(directionalLights.size()); // We need to cast to int because the uniform is an int, otherwise we'd get a warning
    shaders->setUniformInt("numDirLights", directionalLightsCount);
    for(int i(0); i<directionalLightsCount; ++i)
    {
        lightSetup(i, *directionalLights[i]);
    }

    auto& pointLights = lights.pointLights;
    int pointLightsCount = static_cast<int>(pointLights.size());
    shaders->setUniformInt("numPointLights", pointLightsCount);
    for(int i(0); i<pointLightsCount; ++i)
    {
        lightSetup(i, *pointLights[i]);
    }

    auto& spotLights = lights.spotLights;
    int spotLightsCount = static_cast<int>(spotLights.size());
    shaders->setUniformInt("numSpotLights", spotLightsCount);
    for(int i(0); i<spotLightsCount; ++i)
    {
        lightSetup(i, *spotLights[i]);
    }

    return true;
}

void LightLibrary::lightSetup(unsigned int lightIndex, const DirectionalLight &light)
{
    auto shaders = _shaderLibrary.lock();

    // Direction
    glm::vec3 direction = conversion::toVec3(light.getDirection());
    shaders->setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].direction", direction);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    shaders->setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].ambient", color);
    shaders->setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].diffuse", color);
    shaders->setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].specular", color);
}

void LightLibrary::lightSetup(unsigned int lightIndex, const PointLight &light)
{
    auto shaders = _shaderLibrary.lock();

    // Position
    glm::vec3 position = conversion::toVec3(light.getPosition());
    shaders->setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].position", position);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    shaders->setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].ambient", color);
    shaders->setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].diffuse", color);
    shaders->setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].specular", color);

    // Attenuation factors
    const std::array<float, 3>& attFactors = light.getAttenuationFactors();
    shaders->setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].constant", attFactors[0]);
    shaders->setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].linear", attFactors[1]);
    shaders->setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].quadratic", attFactors[2]);

    // Shadow Maps - Light Space Matrix
    if( _ranFrom->getSettings()->getShadowGlobal() == E_Setting::OFF ||
        _ranFrom->getSettings()->getShadowPoint() == E_Setting::OFF)
    {
        return;
    }

    if(_shadowMaps.find(light._id) == _shadowMaps.end())
    {
        return;
        throw std::runtime_error("Shadow map not found");
    }

    ShadowMap& shaMap = _shadowMaps[light._id];
    shaders->setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].farPlane", shaMap._farPlane);
    shaders->setUniformInt("pointLight[" + std::to_string(lightIndex) + "].shadowMap", 15 + lightIndex);

    glActiveTexture(GL_TEXTURE0 + 15 + lightIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shaMap.getShadowMap()->getDepthTextureID());
    glActiveTexture(GL_TEXTURE0);
}

void LightLibrary::lightSetup(unsigned int lightIndex, const SpotLight &light)
{
    auto shaders = _shaderLibrary.lock();

       // Position
    glm::vec3 position = conversion::toVec3(light.getPosition());
    shaders->setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].position", position);

    // Direction
    glm::vec3 direction = conversion::toVec3(light.getDirection());
    shaders->setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].direction", direction);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    shaders->setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].ambient", color);
    shaders->setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].diffuse", color);
    shaders->setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].specular", color);

    // Attenuation factors
    const std::array<float, 3>& attFactors = light.getAttenuationFactors();
    shaders->setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].constant", attFactors[0]);
    shaders->setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].linear", attFactors[1]);
    shaders->setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].quadratic", attFactors[2]);

    // Cutoff
    std::array<float, 2> cutoff = light.getCutoff();
    shaders->setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].innerCutOff", glm::cos(glm::radians(cutoff[0])));
    shaders->setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].outerCutOff", glm::cos(glm::radians(cutoff[1])));

    // Shadow Maps - Light Space Matrix

    if( _ranFrom->getSettings()->getShadowGlobal() == E_Setting::OFF ||
        _ranFrom->getSettings()->getShadowSpot() == E_Setting::OFF)
    {
        return;
    }

    if(_shadowMaps.find(light._id) == _shadowMaps.end())
    {
        return;
        throw std::runtime_error("Shadow map not found");
    }
    
    ShadowMap& shaMap = _shadowMaps[light._id];
    shaders->setUniformMat4("spotLightSpaceMatrix[" + std::to_string(lightIndex) + "]", shaMap.getLightSpaceMatrix());
    shaders->setUniformInt("spotLight[" + std::to_string(lightIndex) + "].shadowMap", 5 + lightIndex);

    glActiveTexture(GL_TEXTURE0 + 5 + lightIndex);
    glBindTexture(GL_TEXTURE_2D, shaMap.getShadowMap()->getDepthTextureID());
    glActiveTexture(GL_TEXTURE0);
}

void LightLibrary::alignShadowMaps(std::shared_ptr<Scene> scene)
{
    auto lights = scene->getAllLights();

    std::vector<std::shared_ptr<LightSource>> allLightSources;
    // allLightSources.insert(allLightSources.end(), lights.directionalLights.begin(), lights.directionalLights.end());
    allLightSources.insert(allLightSources.end(), lights.pointLights.begin(), lights.pointLights.end());
    allLightSources.insert(allLightSources.end(), lights.spotLights.begin(), lights.spotLights.end());

    auto framebuffers = _framebufferManager.lock();

    for(auto& light : allLightSources)
    {   
        bool matchFound = false;
        for(auto& shadowMap : _shadowMaps)
        {
            if(shadowMap.first == light->_id)
            {
                matchFound = true;
                shadowMap.second.alignShadowMap(light);
            }
        }
        if(!matchFound)
        {
            std::shared_ptr<FBO> fbo;
            ShadowMap shadowMap;
            unsigned int shadowMapResolution = getShadowResolution(_ranFrom);

            switch (getLightType(*light))
            {
                case E_LightType::POINT_LIGHT:
                    fbo = framebuffers->addFBO(E_AttachmentTemplate::SHADOW_DEPTH_CUBE, shadowMapResolution, shadowMapResolution);
                    shadowMap.setLightType(E_LightType::POINT_LIGHT);
                    break;
                case E_LightType::SPOT_LIGHT:
                    fbo = framebuffers->addFBO(E_AttachmentTemplate::SHADOW_DEPTH, shadowMapResolution, shadowMapResolution);
                    shadowMap.setLightType(E_LightType::SPOT_LIGHT);
                    break;
            }

            fbo->addAttachment(E_AttachmentSlot::DEPTH);
            shadowMap.setShadowBuffer(fbo);
            shadowMap.alignShadowMap(light);
            shadowMap.setDimensions(shadowMapResolution);
            _shadowMaps.insert(std::make_pair(light->_id, shadowMap));
        }
    }
}

void LightLibrary::renderShadowMaps(std::shared_ptr<Scene> scene)
{
    auto lights = scene->getAllLights();

    std::vector<std::shared_ptr<LightSource>> allLightSources;
    //allLightSources.insert(allLightSources.end(), lights.directionalLights.begin(), lights.directionalLights.end());
    allLightSources.insert(allLightSources.end(), lights.pointLights.begin(), lights.pointLights.end());
    allLightSources.insert(allLightSources.end(), lights.spotLights.begin(), lights.spotLights.end());

    int count = 0;

    for(auto& light : allLightSources)
    {
        switch (getLightType(*light))
        {
            case E_LightType::POINT_LIGHT:
                renderCubeShadowMap(scene, light);
                break;

            case E_LightType::SPOT_LIGHT:
                renderTextureShadowMap(scene, light);
                break;
        }
    }
}

void LightLibrary::renderTextureShadowMap(std::shared_ptr<Scene> scene, std::shared_ptr<LightSource> light)
{
    auto shaders = _shaderLibrary.lock();
    auto framebuffers = _framebufferManager.lock();

    if(shaders == nullptr)
    {
        throw std::runtime_error("Shader Library not bound");
    }
    if(framebuffers == nullptr)
    {
        throw std::runtime_error("Framebuffer Manager not bound");
    }

    // Activate the proper shader
    auto shadowMapperShaders = shaders->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_SHADOW_MAPPING);
    if(shaders->getActiveShaderIndex() != *shadowMapperShaders.begin())
    {
       shaders->use(*shadowMapperShaders.begin());  
    }

    if(_shadowMaps.find(light->_id) == _shadowMaps.end())
    {
        throw std::runtime_error("Shadow map not found");
    }

    ShadowMap& shaMap = _shadowMaps[light->_id];

    shaders->setUniformMat4("lightSpaceMatrix", shaMap.getLightSpaceMatrix());

    auto FBO_INDEX = framebuffers->getFBOIndex(shaMap.getShadowMap());
    glViewport(0, 0, shaMap._bufferWidth, shaMap._bufferHeight);
    framebuffers->bindFBO(FBO_INDEX);
    framebuffers->clearDepth();

    // Render loop
    for (auto& model_vector : scene->getAllModels().getAllModels())
    {
        for(auto model : model_vector.second)
        {
            shaders->setUniformMat4("shadowModel", model->getModelMatrix());

            for (auto &one_mesh : model->getModel()->meshes)
            {
                glBindVertexArray(one_mesh->VAO);
                int numVertexes = static_cast<int>(one_mesh->_indices.size());  // Avoids compiler warning
                glDrawElements(GL_TRIANGLES, numVertexes, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }

    framebuffers->unbindFBO();
}

void LightLibrary::renderCubeShadowMap(std::shared_ptr<Scene> scene, std::shared_ptr<LightSource> light)
{
    auto shaders = _shaderLibrary.lock();
    auto framebuffers = _framebufferManager.lock();
    if(shaders == nullptr)
    {
        throw std::runtime_error("Shader Library not bound");
    }
    if(framebuffers == nullptr)
    {
        throw std::runtime_error("Framebuffer Manager not bound");
    }

    // Activate the proper shader
    auto shadowMapperShaders = shaders->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_SHADOW_CUBE_MAPPING);
    if(shaders->getActiveShaderIndex() != *shadowMapperShaders.begin())
    {
       shaders->use(*shadowMapperShaders.begin());  
    }

    // Check if the light about to be rendered has an allocated framebuffer
    if(_shadowMaps.find(light->_id) == _shadowMaps.end())
    {
        throw std::runtime_error("Shadow map not found");
    }

    ShadowMap& shaMap = _shadowMaps[light->_id];

    auto light_point_position = light->getPosition();

    shaders->setUniformVec3("lightPos", conversion::toVec3(light->getPosition()));
    shaders->setUniformFloat("far_plane", shaMap._farPlane);
    
    for(unsigned int i(0); i<6; ++i)
    {
        shaders->setUniformMat4("lightSpaceMatrix[" + std::to_string(i) + "]", shaMap.getLightSpaceMatrix(i));
    }

    auto FBO_INDEX = framebuffers->getFBOIndex(shaMap.getShadowMap());
    glViewport(0, 0, shaMap._bufferWidth, shaMap._bufferHeight);
    framebuffers->bindFBO(FBO_INDEX);
    framebuffers->clearDepth();

    // Render loop
    for (auto& model_vector : scene->getAllModels().getAllModels())
    {
        for(auto model : model_vector.second)
        {
            shaders->setUniformMat4("model", model->getModelMatrix());

            for (auto &one_mesh : model->getModel()->meshes)
            {
                glBindVertexArray(one_mesh->VAO);
                int numVertexes = static_cast<int>(one_mesh->_indices.size());  // Avoids compiler warning
                glDrawElements(GL_TRIANGLES, numVertexes, GL_UNSIGNED_INT, 0);  
                glBindVertexArray(0);
            }
        }
    }
    framebuffers->unbindFBO();

}   
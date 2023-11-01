#include "rendering/engineModules/LightManager.hpp"

#include <stdexcept>


namespace
{
    E_LightType getType(const LightSource& light)
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

    switch(_lightType)
    {
        case E_LightType::DIRECTIONAL_LIGHT:
            if(!std::dynamic_pointer_cast<DirectionalLight>(light))
            {
                throw std::runtime_error("Light type mismatch");
            }
            break;
        case E_LightType::POINT_LIGHT:
            if(!std::dynamic_pointer_cast<PointLight>(light))
            {
                throw std::runtime_error("Light type mismatch");
            }
            break;
        case E_LightType::SPOT_LIGHT:
            if(!std::dynamic_pointer_cast<SpotLight>(light))
            {
                throw std::runtime_error("Light type mismatch");
            }
            break;
        default:
            throw std::runtime_error("Light type not recognized");
    }

   // Calculate light space transform

    float zNear = 1.0f;
    float zFar = 100.0f;

    light->getPosition();
   switch(_lightType)
   {
    case E_LightType::DIRECTIONAL_LIGHT:
        _lightSpaceMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, zNear, zFar) * glm::lookAt(
            conversion::toVec3(light->getPosition()) * 100.0f, 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
        break;	
    case E_LightType::POINT_LIGHT:
    case E_LightType::SPOT_LIGHT:
        _lightSpaceMatrix = glm::perspective(360.0f, 1.0f, zNear, zFar) * glm::lookAt(
            conversion::toVec3(light->getPosition()), 
            glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 1.0f, 0.0f)
            );
        break;    
   } 
}

std::shared_ptr<FBO> ShadowMap::getShadowMap() const
{
    return _shadowDepthBuffer.lock();
}

const glm::mat4& ShadowMap::getLightSpaceMatrix() const
{
    return _lightSpaceMatrix;
}

void ShadowMap::setLightType(E_LightType type)
{
    _lightType = type;
}

void ShadowMap::setShadowBuffer(std::shared_ptr<FBO> shadowMap)
{
    _shadowDepthBuffer = shadowMap;
}

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// LIGHT LIBRARY
///////////////////////////////////////////////////////////////////////////////////////////

LightLibrary::LightLibrary()
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

    auto directionalLights = lights.directionalLights;
    shaders->setUniformInt("numDirLights", directionalLights.size());
    for(unsigned int i(0); i<directionalLights.size(); ++i)
    {
        lightSetup(i, *directionalLights[i]);
    }

    auto pointLights = lights.pointLights;
    shaders->setUniformInt("numPointLights", pointLights.size());
    for(unsigned int i(0); i<pointLights.size(); ++i)
    {
        lightSetup(i, *pointLights[i]);
    }

    auto spotLights = lights.spotLights;
    shaders->setUniformInt("numSpotLights", spotLights.size());
    for(unsigned int i(0); i<spotLights.size(); ++i)
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
}

void LightLibrary::alignShadowMaps(std::shared_ptr<Scene> scene)
{
    auto lights = scene->getAllLights();

    std::vector<std::shared_ptr<LightSource>> allLightSources;
    allLightSources.insert(allLightSources.end(), lights.directionalLights.begin(), lights.directionalLights.end());
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
            auto FBO = framebuffers->addFBO(E_AttachmentFormat::SHADOW_DEPTH, 2048, 2048);
            ShadowMap shadowMap;
            shadowMap.setLightType(getType(*light));
            shadowMap.setShadowBuffer(FBO);
            shadowMap.alignShadowMap(light);

            _shadowMaps.insert(std::make_pair(light->_id, shadowMap));
        }
    }

}

void LightLibrary::renderShadowMaps(std::shared_ptr<Scene> scene)
{

    auto shaders = _shaderLibrary.lock();
    auto framebuffers = _framebufferManager.lock();

    if(shaders == nullptr)
    {
        throw std::runtime_error("Shader Library not bound");
    }

    auto lights = scene->getAllLights();

    std::vector<std::shared_ptr<LightSource>> allLightSources;
    allLightSources.insert(allLightSources.end(), lights.directionalLights.begin(), lights.directionalLights.end());
    allLightSources.insert(allLightSources.end(), lights.pointLights.begin(), lights.pointLights.end());
    allLightSources.insert(allLightSources.end(), lights.spotLights.begin(), lights.spotLights.end());

    // Activate the proper shader
    auto shadowMapperShaders = shaders->getShaderIndexesPerFeature(E_ShaderProgramFeatures::E_SHADOW_MAPPING);
    shaders->use(*shadowMapperShaders.begin());  

    for(auto& light : allLightSources)
    {
        if(_shadowMaps.find(light->_id) == _shadowMaps.end())
        {
            throw std::runtime_error("Shadow map not found");
        }

        ShadowMap& shaMap = _shadowMaps[light->_id];
        auto FBO_INDEX = framebuffers->getFBOIndex(shaMap.getShadowMap());
        framebuffers->bindFBO(FBO_INDEX);
        framebuffers->clearDepth();

            for (auto& model_vector : scene->getAllModels().getAllModels())
            {
                for(auto model : model_vector.second)
                {
                    for (auto &one_mesh : model->getModel()->meshes)
                    {
                        glBindVertexArray(one_mesh->VAO);
                        glDrawElements(GL_TRIANGLES, one_mesh->_indices.size(), GL_UNSIGNED_INT, 0);
                        glBindVertexArray(0);
                    }
                }
            }
        framebuffers->unbindFBO();
    }

}


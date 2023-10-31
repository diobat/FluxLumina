#include "rendering/engineModules/LightManager.hpp"



bool LightLibrary::prepare(ShaderLibrary& shaders, const LightContents& lights)
{
    auto directionalLights = lights.directionalLights;
    shaders.setUniformInt("numDirLights", directionalLights.size());
    for(unsigned int i(0); i<directionalLights.size(); ++i)
    {
        lightSetup(shaders, i, *directionalLights[i]);
    }

    auto pointLights = lights.pointLights;
    shaders.setUniformInt("numPointLights", pointLights.size());
    for(unsigned int i(0); i<pointLights.size(); ++i)
    {
        lightSetup(shaders, i, *pointLights[i]);
    }

    auto spotLights = lights.spotLights;
    shaders.setUniformInt("numSpotLights", spotLights.size());
    for(unsigned int i(0); i<spotLights.size(); ++i)
    {
        lightSetup(shaders, i, *spotLights[i]);
    }

    return true;
}


void LightLibrary::lightSetup(ShaderLibrary& shaders, unsigned int lightIndex, const DirectionalLight &light)
{
    // Direction
    glm::vec3 direction = conversion::toVec3(light.getDirection());
    shaders.setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].direction", direction);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    shaders.setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].ambient", color);
    shaders.setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].diffuse", color);
    shaders.setUniformVec3("dirLight[" + std::to_string(lightIndex) + "].specular", color);
}

void LightLibrary::lightSetup(ShaderLibrary& shaders, unsigned int lightIndex, const PointLight &light)
{
    // Position
    glm::vec3 position = conversion::toVec3(light.getPosition());
    shaders.setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].position", position);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    shaders.setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].ambient", color);
    shaders.setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].diffuse", color);
    shaders.setUniformVec3("pointLight[" + std::to_string(lightIndex) + "].specular", color);

    // Attenuation factors
    const std::array<float, 3>& attFactors = light.getAttenuationFactors();
    shaders.setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].constant", attFactors[0]);
    shaders.setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].linear", attFactors[1]);
    shaders.setUniformFloat("pointLight[" + std::to_string(lightIndex) + "].quadratic", attFactors[2]);
}

void LightLibrary::lightSetup(ShaderLibrary& shaders, unsigned int lightIndex, const SpotLight &light)
{
       // Position
    glm::vec3 position = conversion::toVec3(light.getPosition());
    shaders.setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].position", position);

    // Direction
    glm::vec3 direction = conversion::toVec3(light.getDirection());
    shaders.setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].direction", direction);

    // Color
    glm::vec3 color = conversion::toVec3(light.getColor());
    shaders.setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].ambient", color);
    shaders.setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].diffuse", color);
    shaders.setUniformVec3("spotLight[" + std::to_string(lightIndex) + "].specular", color);

    // Attenuation factors
    const std::array<float, 3>& attFactors = light.getAttenuationFactors();
    shaders.setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].constant", attFactors[0]);
    shaders.setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].linear", attFactors[1]);
    shaders.setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].quadratic", attFactors[2]);

    // Cutoff
    std::array<float, 2> cutoff = light.getCutoff();
    shaders.setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].innerCutOff", glm::cos(glm::radians(cutoff[0])));
    shaders.setUniformFloat("spotLight[" + std::to_string(lightIndex) + "].outerCutOff", glm::cos(glm::radians(cutoff[1])));
}
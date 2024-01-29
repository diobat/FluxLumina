#pragma once

// STL includes
#include <array>
#include <vector>
#include <memory>
#include <string>

// Third-party includes
#include <glm/glm.hpp>

class StrategyChain;
class FBO;
class Shader;

class StrategyNode
{
public:
    StrategyNode(const StrategyChain* chain);
    virtual void run() = 0;
protected:
    const StrategyChain* _chain;
};

class ViewportUpdateNote : public StrategyNode
{
public:
    ViewportUpdateNote(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};


class CameraSetupNode : public StrategyNode
{
public:
    CameraSetupNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class ShadowsSetupNode : public StrategyNode
{
public:
    ShadowsSetupNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class LightsSetupNode : public StrategyNode
{
public:
    LightsSetupNode(const StrategyChain* chain);
    void run() override;
};

class FramebufferNode : public StrategyNode
{
public:
    FramebufferNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class RenderOpaqueNode : public StrategyNode
{
public:
    RenderOpaqueNode(const StrategyChain* chain);
    void run() override;
};

class RenderOpaqueNodeInstanced : public StrategyNode
{
public:
    RenderOpaqueNodeInstanced(const StrategyChain* chain);
    void run() override;
};

class RenderSkyboxNode : public StrategyNode
{
public:
    RenderSkyboxNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class RenderTransparentNode : public StrategyNode
{
public:
    RenderTransparentNode(const StrategyChain* chain, std::shared_ptr<Shader> transparencyShader = nullptr);
    void run() override;
private:
    std::shared_ptr<Shader> _transparencyShader;
};

class BloomNode : public StrategyNode
{
public:
    BloomNode(const StrategyChain* chain);
    ~BloomNode();
    void run() override;
private:
    std::array<std::shared_ptr<FBO>, 2> _pingPongFBOs;
};

class HighDynamicRangeNode : public StrategyNode
{
public:
    HighDynamicRangeNode(const StrategyChain* chain);
    void run() override;
private:
    std::shared_ptr<FBO> _HDRfbo;
};

class DefaultFramebufferNode : public StrategyNode
{
public:
    DefaultFramebufferNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFERRED SHADING NODES
///////////////////////////////////////////////////////////////////////////////////////////

class GeometryPassNode : public StrategyNode
{
public:
    GeometryPassNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class LightPassNode : public StrategyNode
{
public:
    LightPassNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class LightVolumeNode : public StrategyNode
{
public:
    LightVolumeNode(const StrategyChain* chain);
    void run() override;
private:    
    std::shared_ptr<FBO> _fbo;
};

class SSAONode : public StrategyNode
{
public:
    SSAONode(const StrategyChain* chain);
    void run() override;
    std::shared_ptr<FBO> getFBO() const;
private:
    unsigned int _noiseTexture;
    std::shared_ptr<FBO> _occlusionFBO;
    std::vector<glm::vec3> _ssaoKernel;
    std::vector<glm::vec3> _ssaoNoise;
};

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PHYSICALLY BASED SHADING NODES
///////////////////////////////////////////////////////////////////////////////////////////

class PBS_renderOpaqueNode : public StrategyNode
{
public:
    PBS_renderOpaqueNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

class PBS_IBLSetupNode : public StrategyNode
{
public:
    PBS_IBLSetupNode(const StrategyChain* chain, const std::string& shaderName);
    void run() override;
private:
    std::string _ShaderName;
};

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEBUG NODES
//////////////////////////////////////////////////////////////////////////////////////////

class LightSourceCubeDebugNode : public StrategyNode
{
public:
    LightSourceCubeDebugNode(const StrategyChain* chain, bool depthTest = true);
    void run() override;
private:
    const bool _depthTest;
};

class RenderCubeMapNode : public StrategyNode
{
public:
    RenderCubeMapNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};
#pragma once

// STL includes
#include <array>
#include <memory>

class StrategyChain;
class FBO;

class StrategyNode
{
public:
    StrategyNode(const StrategyChain* chain);
    virtual void run() = 0;
protected:
    const StrategyChain* _chain;
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
    LightsSetupNode(const StrategyChain* chain) : StrategyNode(chain) {}
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
    RenderOpaqueNode(const StrategyChain* chain) : StrategyNode(chain) {}
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
    RenderTransparentNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
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
    HighDynamicRangeNode(const StrategyChain* chain) : StrategyNode(chain) {};
    void run() override;
private:
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

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEBUG NODES
//////////////////////////////////////////////////////////////////////////////////////////

class LightSourceCubeDebugNode : public StrategyNode
{
public:
    LightSourceCubeDebugNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};

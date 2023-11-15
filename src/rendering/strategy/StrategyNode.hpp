#pragma once

class StrategyChain;

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

class HighDynamicRangeNode : public StrategyNode
{
public:
    HighDynamicRangeNode(const StrategyChain* chain) : StrategyNode(chain) {}
    void run() override;
};
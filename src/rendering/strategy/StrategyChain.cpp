#include "rendering/strategy/StrategyChain.hpp"

StrategyChain::StrategyChain(GraphicalEngine* engine) : 
    _ranFrom(engine)
{
    ;
}

bool StrategyChain::add(std::shared_ptr<StrategyNode> node)
{
    _nodes.push_back(node);
    return true;
}

void StrategyChain::clear()
{
    _nodes.clear();
}

void StrategyChain::run()
{
    for (auto& node : _nodes)
    {
        node->run();
    }
}

GraphicalEngine* StrategyChain::engine() const
{
    return _ranFrom;
}

DefaultStrategyChain::DefaultStrategyChain(GraphicalEngine* engine) : 
    StrategyChain(engine)
{
    add(std::make_shared<CameraSetupNode>(this));
    add(std::make_shared<ShadowsSetupNode>(this));
    add(std::make_shared<LightsSetupNode>(this));
    add(std::make_shared<FramebufferNode>(this));
    add(std::make_shared<RenderOpaqueNode>(this));
    add(std::make_shared<RenderSkyboxNode>(this));
    add(std::make_shared<RenderTransparentNode>(this));
}
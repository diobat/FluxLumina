#pragma once

// STL headers
#include <list>
#include <memory>
#include <tuple>

// First-party headers
#include "rendering/strategy/StrategyNode.hpp"


class GraphicalEngine;

class StrategyChain
{
public:
    StrategyChain(GraphicalEngine* engine);

    bool add(std::shared_ptr<StrategyNode> node);
    void clear();
    virtual bool reserveResources() { return true;};
    void run();

    GraphicalEngine* engine() const;

protected:
    std::list<std::shared_ptr<StrategyNode>> _nodes;

    // The engine currently running this chain
    GraphicalEngine* _ranFrom;

private:
    bool _firstRun;
};

class ForwardShadingStrategyChain : public StrategyChain
{
public:
    ForwardShadingStrategyChain(GraphicalEngine* engine);
};

class DeferredShadingStrategyChain : public StrategyChain
{
public:
    DeferredShadingStrategyChain(GraphicalEngine* engine);
    bool reserveResources() override;
};



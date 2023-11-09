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
    void run();

    GraphicalEngine* engine() const;

private:
    std::list<std::shared_ptr<StrategyNode>> _nodes;

    // The engine currently running this chain
    GraphicalEngine* _ranFrom;
};

class DefaultStrategyChain : public StrategyChain
{
public:
    DefaultStrategyChain(GraphicalEngine* engine);
};

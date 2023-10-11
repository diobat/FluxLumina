#pragma once

// STL includes
#include <list>


class StrategyNode
{
public:

    StrategyNode();
    virtual ~StrategyNode() = 0{};

    virtual void execute() = 0{};
};

class RenderStrategy
{
public:

    RenderStrategy();

    void addNode(StrategyNode node);

private:
std::list<StrategyNode> _nodes;
};


class RenderStrategyNode : public StrategyNode
{   
    
};
#pragma once

// STL headers
#include <list>
#include <memory>
#include <tuple>

// First-party headers
#include "rendering/strategy/StrategyNode.hpp"

class GraphicalEngine;
class Shader;

class StrategyChain
{
public:
    StrategyChain(FluxLumina* engine);

    bool add(std::shared_ptr<StrategyNode> node);
    void clear();
    virtual bool reserveResources() { return true;};
    void run();

    FluxLumina* engine() const;

    template<typename T>
    std::shared_ptr<T> getNode() const
    {
    for (auto& node : _nodes)
    {
        const auto casted = std::dynamic_pointer_cast<T>(node);
        if (casted)
        {
            return casted;
        }
    }
    return nullptr;
    };

protected:
    std::list<std::shared_ptr<StrategyNode>> _nodes;

    // The engine currently running this chain
    FluxLumina* _ranFrom;

private:
    bool _firstRun;
};

class ForwardShadingStrategyChain : public StrategyChain
{
public:
    ForwardShadingStrategyChain(GraphicalEngine* engine, std::shared_ptr<Shader> instancingShader = nullptr, std::shared_ptr<Shader> transparencyShader = nullptr);
    bool reserveResources() override;
};

class DeferredShadingStrategyChain : public StrategyChain
{
public:
    DeferredShadingStrategyChain(FluxLumina* engine);
    bool reserveResources() override;
};

class PBSShadingStrategyChain : public StrategyChain
{
public:
    PBSShadingStrategyChain(FluxLumina* engine);
    bool reserveResources() override;
};
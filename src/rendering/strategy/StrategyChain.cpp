#include "rendering/strategy/StrategyChain.hpp"

#include "rendering/GraphicalEngine.hpp"
#include "rendering/Settings.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"

StrategyChain::StrategyChain(GraphicalEngine* engine) : 
    _ranFrom(engine),
    _firstRun(true)
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

    if(_firstRun)
    {
        reserveResources();
        _firstRun = false;
    }

    for (auto& node : _nodes)
    {
        node->run();
    }
}

GraphicalEngine* StrategyChain::engine() const
{
    return _ranFrom;
}

ForwardShadingStrategyChain::ForwardShadingStrategyChain(GraphicalEngine* engine) : 
    StrategyChain(engine)
{
        // Setups
        add(std::make_shared<CameraSetupNode>(this));
        add(std::make_shared<ShadowsSetupNode>(this));
        add(std::make_shared<LightsSetupNode>(this));
        add(std::make_shared<FramebufferNode>(this));
        // Rendering
        add(std::make_shared<RenderOpaqueNode>(this));
        add(std::make_shared<RenderSkyboxNode>(this));
        add(std::make_shared<RenderTransparentNode>(this));
        // Post-processing
        if(_ranFrom->getSettings()->getBloom() == E_Setting::ON)
        {
            add(std::make_shared<BloomNode>(this));  
        }
        if(_ranFrom->getSettings()->getHighDynamicRange() == E_Setting::ON)
        {
            add(std::make_shared<HighDynamicRangeNode>(this));  
        }
        // Move to default Framebuffer frame
        add(std::make_shared<DefaultFramebufferNode>(this));     
}


DeferredShadingStrategyChain::DeferredShadingStrategyChain(GraphicalEngine* engine) : 
    StrategyChain(engine)
{   
    // Setups
    add(std::make_shared<CameraSetupNode>(this));
    add(std::make_shared<LightsSetupNode>(this));
    add(std::make_shared<FramebufferNode>(this));

    // Rendering
    add(std::make_shared<RenderSkyboxNode>(this));
    add(std::make_shared<GeometryPassNode>(this));
    add(std::make_shared<LightPassNode>(this));

    // Post-processing
    if(_ranFrom->getSettings()->getBloom() == E_Setting::ON)
    {
        add(std::make_shared<BloomNode>(this));  
    }
    if(_ranFrom->getSettings()->getHighDynamicRange() == E_Setting::ON)
    {
        add(std::make_shared<HighDynamicRangeNode>(this));  
    }

    add(std::make_shared<LightSourceCubeDebugNode>(this));
    // Move to default Framebuffer frame
    add(std::make_shared<DefaultFramebufferNode>(this));     
}


bool DeferredShadingStrategyChain::reserveResources()
{
    std::shared_ptr<FBOManager> framebufferManager = _ranFrom->getFBOManager();
    std::shared_ptr<FBO> FBO = framebufferManager->getSceneFBO(_ranFrom->getScene());

    FBO->reset();  
    // Create G-Buffer
    FBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);    // Color Output

    FBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);    // Position
    FBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);    // Normals
    FBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA);       // Albedo + Specular

    FBO->addAttachment(E_AttachmentSlot::DEPTH);                            // Depth

    return true;
}
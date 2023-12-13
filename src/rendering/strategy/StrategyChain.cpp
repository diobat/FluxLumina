#include "rendering/strategy/StrategyChain.hpp"

#include "rendering/GraphicalEngine.hpp"
#include "rendering/Settings.hpp"
#include "rendering/framebuffer/Framebuffer_Manager.hpp"

#include "util/VertexShapes.hpp"

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

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// FORWARD
///////////////////////////////////////////////////////////////////////////////////////////

ForwardShadingStrategyChain::ForwardShadingStrategyChain(GraphicalEngine* engine) : 
    StrategyChain(engine)
{
        // Setups
        add(std::make_shared<CameraSetupNode>(this));
        add(std::make_shared<ShadowsSetupNode>(this));
        add(std::make_shared<LightsSetupNode>(this, "Basic"));
        add(std::make_shared<FramebufferNode>(this));
        // Rendering
        add(std::make_shared<RenderSkyboxNode>(this));
        add(std::make_shared<RenderOpaqueNode>(this));
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

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFERRED
///////////////////////////////////////////////////////////////////////////////////////////

DeferredShadingStrategyChain::DeferredShadingStrategyChain(GraphicalEngine* engine) : 
    StrategyChain(engine)
{   
    // Setupsa
    add(std::make_shared<CameraSetupNode>(this));
    add(std::make_shared<LightsSetupNode>(this, "deferred_light_volumes"));
    add(std::make_shared<FramebufferNode>(this));

    // Rendering
    add(std::make_shared<RenderSkyboxNode>(this));
    add(std::make_shared<GeometryPassNode>(this));
    // add(std::make_shared<LightSourceCubeDebugNode>(this, true));
    if(_ranFrom->getSettings()->getSSAO() == E_Setting::ON)
    {
        add(std::make_shared<SSAONode>(this));
    }
    add(std::make_shared<LightPassNode>(this));
    add(std::make_shared<LightVolumeNode>(this));

    // Post-processing
    if(_ranFrom->getSettings()->getHighDynamicRange() == E_Setting::ON)
    {
        add(std::make_shared<HighDynamicRangeNode>(this));  
    }

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
    FBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);    // Screen Space Position
    FBO->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);    // Screen Space Normals

    FBO->addAttachment(E_AttachmentSlot::DEPTH);                            // Depth

    return true;
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PHYSICALLY BASED
///////////////////////////////////////////////////////////////////////////////////////////

PBSShadingStrategyChain::PBSShadingStrategyChain(GraphicalEngine* engine) : 
    StrategyChain(engine)
{   
    // Setups
    add(std::make_shared<CameraSetupNode>(this));
    add(std::make_shared<LightsSetupNode>(this, "PBR_basic"));
    add(std::make_shared<PBS_IBLSetupNode>(this, "PBR_basic"));
    add(std::make_shared<FramebufferNode>(this));

    // Rendering
    add(std::make_shared<RenderSkyboxNode>(this));
    add(std::make_shared<RenderOpaqueNode>(this));
    //add(std::make_shared<RenderCubeMapNode>(this));

    // Post-processing
    if(_ranFrom->getSettings()->getHighDynamicRange() == E_Setting::ON)
    {
        add(std::make_shared<HighDynamicRangeNode>(this));  
    }

    // Move to default Framebuffer frame
    add(std::make_shared<DefaultFramebufferNode>(this));     
}

bool PBSShadingStrategyChain::reserveResources()
{
    std::shared_ptr<ShaderLibrary> shaderLibrary = _ranFrom->getShaderLibrary();
    std::shared_ptr<LightLibrary> lightLibrary = _ranFrom->getLightLibrary();

    lightLibrary->getLightMap().init(2048u);
    unsigned int skyboxTextureID = lightLibrary->getLightMap().bakeFromTexture(_ranFrom->getScene()->getSkybox().getIBLmap());
    lightLibrary->getLightMap().convoluteLightMap();

    std::shared_ptr<Cubemap> cubemap = _ranFrom->getScene()->getSkybox().getCubemap();
    cubemap->getTexture()._id = skyboxTextureID;
    cubemap->VAO = shapes::cube::VAO();

    return true;
}
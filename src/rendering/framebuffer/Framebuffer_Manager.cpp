#include "rendering/framebuffer/Framebuffer_Manager.hpp"

#include "rendering/GraphicalEngine.hpp"

FBOManager::FBOManager(GraphicalEngine *engine) : 
    _ranFrom(engine),
    _currentFBOIndex(-1),
    _forceDefault(false)
{
    for(std::shared_ptr<Scene> scene : engine->getScenes())
    {
        parseNewScene(scene);
    }
}

FBOManager::~FBOManager()
{
    ;
}

std::shared_ptr<FBO> FBOManager::addFBO(E_AttachmentTemplate format, int width, int height)
{
    std::shared_ptr<FBO> fbo;

    fbo = std::make_shared<FBO>(format, width, height);

    _frameBufferObjects.push_back(fbo);
    return fbo;
}

void FBOManager::removeFBO(std::shared_ptr<FBO> fbo)
{
    unsigned int id = fbo->id();
    glDeleteFramebuffers(1, &id);
    
    for (unsigned int i = 0; i < _frameBufferObjects.size(); i++)
    {
        if (_frameBufferObjects[i] == fbo)
        {
            _frameBufferObjects.erase(_frameBufferObjects.begin() + i);
            return;
        }
    }
}

void FBOManager::bindFBO(unsigned int fboIndex)
{
    if(fboIndex >= _frameBufferObjects.size())
    {
        return;
    }

    _currentFBOIndex = fboIndex;
    unsigned int newID = _frameBufferObjects[fboIndex]->id();
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObjects[fboIndex]->id());
}

void FBOManager::bindFBO(std::shared_ptr<FBO> fbo)
{
    bindFBO(getFBOIndex(fbo));
}

void FBOManager::unbindFBO()
{
    _currentFBOIndex = -1;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

size_t FBOManager::getFBOCount() const
{
    return _frameBufferObjects.size();
}

std::shared_ptr<FBO> FBOManager::getFBO(unsigned int fboIndex) const
{
    if(fboIndex >= _frameBufferObjects.size())
    {
        return nullptr;
    }

    return _frameBufferObjects[fboIndex];
}

std::shared_ptr<FBO> FBOManager::getSceneFBO(std::shared_ptr<Scene> scene) const
{
    if(_fboSceneMap.find(scene) == _fboSceneMap.end())
    {
        return nullptr;
    }

    return _fboSceneMap.at(scene);
}

unsigned int FBOManager::getFBOIndex(std::shared_ptr<FBO> fbo) const
{
    for (unsigned int i = 0; i < _frameBufferObjects.size(); i++)
    {
        if (_frameBufferObjects[i] == fbo)
        {
            return i;
        }
    }

    return -1;
}

void FBOManager::parseNewScene(std::shared_ptr<Scene> scene)
{
    if(_fboSceneMap.count(scene) == 0)
    {
        std::shared_ptr<FBO> fbo = addFBO(E_AttachmentTemplate::TEXTURE, _ranFrom->getViewportSize()[0], _ranFrom->getViewportSize()[1]);
        fbo->bindToViewportSize(true);
        fbo->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);
        if(_ranFrom->getSettings()->getBloom() == E_Setting::ON)
        {
            fbo->addAttachment(E_AttachmentSlot::COLOR, E_ColorFormat::RGBA16F);
        }
        fbo->addAttachment(E_AttachmentSlot::DEPTH);
        //fbo->addAttachment(E_AttachmentSlot::STENCIL);

        bindSceneToFBO(scene, fbo);
        bindProperFBOFromScene(scene);
    }
}

bool FBOManager::bindSceneToFBO(std::shared_ptr<Scene> scene, std::shared_ptr<FBO> fbo)
{
    if(fbo == nullptr)
    {
        if(_fboSceneMap.find(scene) != _fboSceneMap.end())
        {
            _fboSceneMap.erase(scene);
            return true;
        }
        return false;
    }

    _fboSceneMap[scene] = fbo;
    return true;
}

bool FBOManager::unbindSceneFromFBO(std::shared_ptr<Scene> scene)
{
    return bindSceneToFBO(scene, nullptr);
}

bool FBOManager::isFrameBufferComplete(std::shared_ptr<FBO> fbo) const
{
    return isFrameBufferComplete(getFBOIndex(fbo));
}

void FBOManager::bindProperFBOFromScene(std::shared_ptr<Scene> scene)
{
    if(_forceDefault)
    {
        unbindFBO();
        return;
    }

    if(_fboSceneMap.find(scene) == _fboSceneMap.end())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    bindFBO(getFBOIndex(_fboSceneMap[scene]));
}

void FBOManager::clearAll()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FBOManager::clearColor()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void FBOManager::clearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void FBOManager::clearStencil()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

bool FBOManager::isFrameBufferComplete(unsigned int fboIndex) const
{
    if(fboIndex >= _frameBufferObjects.size())
    {
        return false;
    }

    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FBOManager::resizeViewPortBoundFBOs()
{

    std::array<int, 2> viewPortSize = _ranFrom->getViewportSize();

    for(std::shared_ptr<FBO> fbo : _frameBufferObjects)
    {
        if(fbo->isBoundToViewportSize())
        {
            fbo->resize(viewPortSize[0], viewPortSize[1]);
        }
    }
}
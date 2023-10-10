#include "rendering/framebuffer/Framebuffer_Manager.h"

FBOManager::FBOManager(GLFWwindow *window) : 
    _currentFBOIndex(-1),
    _window(window)
{
    ;
}

FBOManager::~FBOManager()
{
    ;
}

std::shared_ptr<FBO> FBOManager::addFBO(E_AttachmentFormat format, int width, int height)
{
    std::shared_ptr<FBO> fbo;

    switch (format)
    {
        case TEXTURE:
            fbo = std::make_shared<TextureFBO>(width, height);
            break;
        case RENDERBUFFER:
            fbo = std::make_shared<RenderBufferFBO>(width, height);
            break;
        default:
            break;
    }
    _frameBufferObjects.push_back(fbo);
    return fbo;
}

void FBOManager::bindFBO(unsigned int fboIndex)
{
    if(fboIndex >= _frameBufferObjects.size())
    {
        return;
    }

    _currentFBOIndex = fboIndex;
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferObjects[fboIndex]->getID());
}

void FBOManager::unbindFBO()
{
    _currentFBOIndex = -1;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int FBOManager::getFBOCount() const
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

bool FBOManager::isFrameBufferComplete(std::shared_ptr<FBO> fbo) const
{
    return isFrameBufferComplete(getFBOIndex(fbo));
}

bool FBOManager::isFrameBufferComplete(unsigned int fboIndex) const
{
    if(fboIndex >= _frameBufferObjects.size())
    {
        return false;
    }

    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}


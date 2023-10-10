#include "rendering/framebuffer/FBO.h"

FBO::FBO(unsigned int width, unsigned int height) : 
    _id(-1),
    _originalSize({width, height}),
    _depthAttachmentID(-1),
    _stencilAttachmentID(-1)
{
    ;
}


unsigned int FBO::addColorAttachment()
{
    unsigned int _textureId;
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getOriginalSize()[0], getOriginalSize()[1], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);

    _colorAttachmentIDs.push_back(_textureId);

    return _textureId;
}

TextureFBO::TextureFBO(unsigned int width, unsigned int height) : 
    FBO(width, height)

{
    glGenFramebuffers(1, &_id);
}

TextureFBO::~TextureFBO()
{

    if(!_colorAttachmentIDs.empty())
    {
        glDeleteTextures(_colorAttachmentIDs.size(), _colorAttachmentIDs.data());
    }
    if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }
    if(_stencilAttachmentID != -1)
    {
        glDeleteTextures(1, &_stencilAttachmentID);
    }

    glDeleteFramebuffers(1, &_id);
}

void TextureFBO::addAttachment(E_AttachmentType type)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    switch(type)
    {
        case E_AttachmentType::COLOR:
            addColorAttachment();
            break;
        case E_AttachmentType::DEPTH:
            addDepthAttachment();
            break;
        case E_AttachmentType::STENCIL:
            addStencilAttachment();
            break;
        default:
            break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureFBO::addDepthAttachment()
{
    if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    _depthAttachmentID = textureId;
}

void TextureFBO::addStencilAttachment()
{
    if(_stencilAttachmentID != -1)
    {
        glDeleteTextures(1, &_stencilAttachmentID);
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, getOriginalSize()[0], getOriginalSize()[1], 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    _stencilAttachmentID = textureId;
}

RenderBufferFBO::RenderBufferFBO(unsigned int width, unsigned int height) : 
    FBO(width, height)
{
    glGenRenderbuffers(1, &_id);
}

RenderBufferFBO::~RenderBufferFBO()
{
    glDeleteRenderbuffers(1, &_id);
}

void RenderBufferFBO::addAttachment(E_AttachmentType type)
{
    unsigned int _attachmentID;
    glBindRenderbuffer(GL_RENDERBUFFER, _id);

    switch(type)
    {
        case E_AttachmentType::COLOR:
            _colorAttachmentIDs.push_back(addColorAttachment());
            break;
        case E_AttachmentType::DEPTH:
            addDepthAttachment();
            break;
        case E_AttachmentType::STENCIL:
            addStencilAttachment();
            break;
        default:
            break;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBufferFBO::addDepthAttachment()
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _id);
}

void RenderBufferFBO::addStencilAttachment()
{
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, getOriginalSize()[0], getOriginalSize()[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _id);
}


#include "rendering/framebuffer/FBO.h"


FBO::FBO(unsigned int width, unsigned int height) : 
    _id(-1),
    _originalSize({width, height}),
    _depthAttachmentID(-1),
    _stencilAttachmentID(-1)
{
    glGenFramebuffers(1, &_id);
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

    return _textureId;
}


std::vector<Texture> FBO::getTextures() const
{
    std::vector<Texture> returnTextures;
    for(unsigned int i = 0; i < _colorAttachmentIDs.size(); ++i)
    {
        Texture tempText;
        tempText._id = _colorAttachmentIDs[i];
        tempText._type = E_TexureType::DIFFUSE;
        tempText._colorChannels = GL_RGB;
        tempText._components = 3;
        tempText._useLinear = true;
        returnTextures.push_back(tempText);
    }

    return returnTextures;
}


TextureFBO::TextureFBO(unsigned int width, unsigned int height) : 
    FBO(width, height)
{
    ;
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
            _colorAttachmentIDs.push_back(addColorAttachment());
            break;
        case E_AttachmentType::DEPTH:
            _depthAttachmentID = addDepthAttachment();
            break;
        case E_AttachmentType::STENCIL:
            _stencilAttachmentID = addStencilAttachment();
            break;
        default:
            break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int TextureFBO::addDepthAttachment()
{
    if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    return textureId;
}

unsigned int TextureFBO::addStencilAttachment()
{
    if(_stencilAttachmentID != -1)
    {
        glDeleteTextures(1, &_stencilAttachmentID);
    }
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, getOriginalSize()[0], getOriginalSize()[1], 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    return textureId;
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
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int RenderBufferFBO::addDepthAttachment()
{
    unsigned int rb_id;
    glGenRenderbuffers(1, &rb_id);
    glBindRenderbuffer(GL_RENDERBUFFER, rb_id);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_id);

    return rb_id;
}

unsigned int RenderBufferFBO::addStencilAttachment()
{
    unsigned int rb_id;
    glGenRenderbuffers(1, &rb_id);
    glBindRenderbuffer(GL_RENDERBUFFER, rb_id);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, getOriginalSize()[0], getOriginalSize()[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb_id);

    return rb_id;
}


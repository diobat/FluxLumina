#include "rendering/framebuffer/FBO.hpp"


FBO::FBO(unsigned int width, unsigned int height) : 
    _id(-1),
    _originalSize({width, height}),
    _depthAttachmentID(-1),
    _stencilAttachmentID(-1)
{
    glGenFramebuffers(1, &_id);
}

unsigned int FBO::addColorAttachment(E_ColorFormat colorFormat)
{
    unsigned int _textureId;
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    switch(colorFormat)
    {
        case E_ColorFormat::RGBA:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getOriginalSize()[0], getOriginalSize()[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            break;
        case E_ColorFormat::RGBA16F:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, getOriginalSize()[0], getOriginalSize()[1], 0, GL_RGBA, GL_FLOAT, NULL);
            break;
        case E_ColorFormat::RGB:
        default:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getOriginalSize()[0], getOriginalSize()[1], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

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

unsigned int FBO::getDepthTextureID() const
{
    return _depthAttachmentID;
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
        int size = static_cast<int>(_colorAttachmentIDs.size());  
        glDeleteTextures(size, _colorAttachmentIDs.data());
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

void TextureFBO::addAttachment(E_AttachmentType type, E_ColorFormat colorFormat)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    switch(type)
    {
        case E_AttachmentType::COLOR:
            _colorAttachmentIDs.push_back(addColorAttachment(colorFormat));
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

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

void RenderBufferFBO::addAttachment(E_AttachmentType type, E_ColorFormat colorFormat)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    switch(type)
    {
        case E_AttachmentType::COLOR:
            _colorAttachmentIDs.push_back(addColorAttachment(colorFormat));
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


ShadowDepthFBO::ShadowDepthFBO(unsigned int width, unsigned int height) : 
    FBO(width, height)
{
    _colorAttachmentIDs.clear();
    _stencilAttachmentID = -1;
}

ShadowDepthFBO::~ShadowDepthFBO()
{
    if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }

    glDeleteFramebuffers(1, &_id);
}

void ShadowDepthFBO::addAttachment(E_AttachmentType type, E_ColorFormat colorFormat)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    switch(type)
    {
        case E_AttachmentType::COLOR:
            break;
        case E_AttachmentType::DEPTH:
            _depthAttachmentID = addDepthAttachment();
            break;
        case E_AttachmentType::STENCIL:
            break;
        default:
            break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int ShadowDepthFBO::addDepthAttachment()
{
        if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);

    glDrawBuffer(GL_NONE);  // Framebuffers have a colorbuffer requirement, but we don't need it. In order to guarantee that at least a Depth buffer is created, we need to specify GL_NONE as the colorbuffer inside this function.
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}


ShadowDepthCubeFBO::ShadowDepthCubeFBO(unsigned int width, unsigned int height) : 
    FBO(width, height)
{
    _colorAttachmentIDs.clear();
    _stencilAttachmentID = -1;
    _depthAttachmentID = -1;
}

ShadowDepthCubeFBO::~ShadowDepthCubeFBO()
{
    if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }

    glDeleteFramebuffers(1, &_id);
}

void ShadowDepthCubeFBO::addAttachment(E_AttachmentType type, E_ColorFormat colorFormat)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    switch(type)
    {
        case E_AttachmentType::COLOR:
            break;
        case E_AttachmentType::DEPTH:
            _depthAttachmentID = addDepthAttachment();
            break;
        case E_AttachmentType::STENCIL:
            break;
        default:
            break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int ShadowDepthCubeFBO::addDepthAttachment()
{
    if(_depthAttachmentID != -1)
    {
        glDeleteTextures(1, &_depthAttachmentID);
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    for(unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);

    glDrawBuffer(GL_NONE);  // Framebuffers have a colorbuffer requirement, but we don't need it. In order to guarantee that at least a Depth buffer is created, we need to specify GL_NONE as the colorbuffer inside this function.
    glReadBuffer(GL_NONE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}
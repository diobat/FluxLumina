#include "rendering/framebuffer/FBO.hpp"

FBO::FBO(E_AttachmentTemplate format, unsigned int width, unsigned int height) : 
    _id(-1),
    _originalSize({width, height}),
    _depthAttachment({-1U, E_AttachmentTypes::NONE}),
    _stencilAttachment({-1U, E_AttachmentTypes::NONE}),
    _isViewPortSizeBound(false)
{
    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    switch(format)
    {
        case E_AttachmentTemplate::TEXTURE:
            init({E_AttachmentTypes::TEXTURE, E_AttachmentTypes::TEXTURE, E_AttachmentTypes::TEXTURE});
            break;
        case E_AttachmentTemplate::RENDERBUFFER:
            init({E_AttachmentTypes::TEXTURE, E_AttachmentTypes::RENDERBUFFER, E_AttachmentTypes::RENDERBUFFER});
            break;
        case E_AttachmentTemplate::SHADOW_DEPTH:
            init({E_AttachmentTypes::NONE, E_AttachmentTypes::TEXTURE, E_AttachmentTypes::NONE});
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            break;
        case E_AttachmentTemplate::SHADOW_DEPTH_CUBE:
            init({E_AttachmentTypes::NONE, E_AttachmentTypes::CUBEMAP, E_AttachmentTypes::NONE});
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            break;
        case E_AttachmentTemplate::LIGHTMAP:
            init({E_AttachmentTypes::CUBEMAP, E_AttachmentTypes::RENDERBUFFER, E_AttachmentTypes::NONE});
            break;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
    reset();
    glDeleteFramebuffers(1, &_id);
}

void FBO::init(const std::array<E_AttachmentTypes, 3>& templateTypes)
{
    reset();
    _framebufferTemplate = templateTypes;
}

void FBO::addAttachment(E_AttachmentSlot type, E_ColorFormat colorFormat)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    switch(type)
    {
        case E_AttachmentSlot::COLOR:
            if(_framebufferTemplate[0] != E_AttachmentTypes::NONE)
            {
                _colorAttachments.push_back(addColorAttachment(_framebufferTemplate[0], colorFormat));
            }
            break;
        case E_AttachmentSlot::DEPTH:
            if(_framebufferTemplate[1] != E_AttachmentTypes::NONE)
            {
                _depthAttachment = addDepthAttachment(_framebufferTemplate[1]);
            }
            break;
        case E_AttachmentSlot::STENCIL:
            if(_framebufferTemplate[2] != E_AttachmentTypes::NONE)
            {
                _stencilAttachment = addStencilAttachment(_framebufferTemplate[2]);
            }
            break;
        default:
            break;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ColorAttachment FBO::addColorAttachment(E_AttachmentTypes type,  E_ColorFormat colorFormat)
{

    unsigned int _textureId;

    unsigned int colorOffset = static_cast<unsigned int>(_colorAttachments.size()); 

    unsigned int internalFormat = GL_RGB;
    unsigned int format = GL_RGB;
    unsigned int dataType = GL_UNSIGNED_BYTE;

    switch(colorFormat)
    {
        case E_ColorFormat::RED:
            internalFormat = GL_RED;
            format = GL_RED;
            break;
        case E_ColorFormat::RGBA:
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            break;
        case E_ColorFormat::RGB16F:
            internalFormat = GL_RGB16F;
            format = GL_RGB;
            dataType = GL_FLOAT;
            break;
        case E_ColorFormat::RGBA16F:
            internalFormat = GL_RGBA16F;
            format = GL_RGBA;
            dataType = GL_FLOAT;
            break;
        case E_ColorFormat::RGB:
        default:
            internalFormat = GL_RGB;
            format = GL_RGB;
            dataType = GL_UNSIGNED_BYTE;
            break;
    }

    switch(type)
    {
        case E_AttachmentTypes::CUBEMAP:

            glGenTextures(1, &_textureId);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _textureId);

            for (unsigned int i = 0; i < 6 ; ++i)
            {
               // note that we store each face with 16 bit floating point values
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, getOriginalSize()[0], getOriginalSize()[1], 0, format, dataType, NULL); 
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            break;

        case E_AttachmentTypes::TEXTURE:
        default:

            glGenTextures(1, &_textureId);
            glBindTexture(GL_TEXTURE_2D, _textureId);

            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, getOriginalSize()[0], getOriginalSize()[1], 0, format, dataType, NULL);
            
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorOffset, GL_TEXTURE_2D, _textureId, 0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 

            glBindTexture(GL_TEXTURE_2D, 0);
            break;
    }

    return ColorAttachment({_textureId, type, GL_COLOR_ATTACHMENT0 + colorOffset, colorFormat});
}

Attachment FBO::addDepthAttachment(E_AttachmentTypes type)
{
    unsigned int attachment_id = 0;
    switch(type)
    {
        case E_AttachmentTypes::TEXTURE:
            if(_depthAttachment.id != -1)
            {
                glDeleteTextures(1, &_depthAttachment.id);
            }
            
            glGenTextures(1, &attachment_id);
            glBindTexture(GL_TEXTURE_2D, attachment_id);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // If this depth texture is inteded of a shadowmap the setup is slightly different, however we don't know that here. So we just set it up as a normal depth texture UNLESS the color attachment is NONE.
            if(_framebufferTemplate[0] == E_AttachmentTypes::NONE)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attachment_id, 0);

            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        case E_AttachmentTypes::RENDERBUFFER:
            if(_depthAttachment.id != -1)
            {
                glDeleteRenderbuffers(1, &_depthAttachment.id);
            }

            glGenRenderbuffers(1, &attachment_id);
            glBindRenderbuffer(GL_RENDERBUFFER, attachment_id);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1]);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, attachment_id);

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            break;
        case E_AttachmentTypes::CUBEMAP:
            if(_depthAttachment.id != -1)
            {
                glDeleteTextures(1, &_depthAttachment.id);
            }
            glGenTextures(1, &attachment_id);
            glBindTexture(GL_TEXTURE_CUBE_MAP, attachment_id);

            for(unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, getOriginalSize()[0], getOriginalSize()[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
            
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, attachment_id, 0);

            glDrawBuffer(GL_NONE);  // Framebuffers have a colorbuffer requirement, but we don't need it. In order to guarantee that at least a Depth buffer is created, we need to specify GL_NONE as the colorbuffer inside this function.
            glReadBuffer(GL_NONE);

            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        case E_AttachmentTypes::NONE:
        default:
            return _depthAttachment;
            break;
    }
    return Attachment({attachment_id, type});	
}

Attachment FBO::addStencilAttachment(E_AttachmentTypes type)
{
    unsigned int id;
    switch(_framebufferTemplate[2])
    {
        case E_AttachmentTypes::TEXTURE:

            if(_stencilAttachment.id != -1)
            {
                glDeleteTextures(1, &_stencilAttachment.id);
            }

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, getOriginalSize()[0], getOriginalSize()[1], 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0);

            glBindTexture(GL_TEXTURE_2D, 0);
            break;

        case E_AttachmentTypes::RENDERBUFFER:
            
            if(_stencilAttachment.id != -1)
            {
                glDeleteRenderbuffers(1, &_stencilAttachment.id);
            }

            glGenRenderbuffers(1, &id);
            glBindRenderbuffer(GL_RENDERBUFFER, id);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, getOriginalSize()[0], getOriginalSize()[1]);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            break;

        case E_AttachmentTypes::NONE:
            return _stencilAttachment;
    }
    return Attachment({id, type});	
}

void FBO::reset()
{
    // Color attachments
    for(auto texture : _colorAttachments)
    {
        glDeleteTextures(1, &texture.id);
    }
    _colorAttachments.clear();

    // Depth attachment
    if(_depthAttachment.id != -1)
    {
        switch(_framebufferTemplate[1])
        {
            case E_AttachmentTypes::TEXTURE:
                glDeleteTextures(1, &_depthAttachment.id);
                break;
            case E_AttachmentTypes::RENDERBUFFER:
                glDeleteRenderbuffers(1, &_depthAttachment.id);
                break;
            default:
                break;
        }
        _depthAttachment.id = -1;
    }

    // Stencil attachment
    if(_stencilAttachment.id != -1)
    {
        switch(_framebufferTemplate[2])
        {
            case E_AttachmentTypes::TEXTURE:
                glDeleteTextures(1, &_stencilAttachment.id);
                break;
            case E_AttachmentTypes::RENDERBUFFER:
                glDeleteRenderbuffers(1, &_stencilAttachment.id);
                break;
            default:
                break;
        }
        _stencilAttachment.id = -1;
    }
}

std::vector<Texture> FBO::getTextures() const
{
    std::vector<Texture> returnTextures;
    for(unsigned int i = 0; i < _colorAttachments.size(); ++i)
    {
        Texture tempText;
        tempText._id = _colorAttachments[i].id;
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
    return _depthAttachment.id;
}

void FBO::bindToViewportSize(bool isBound)
{
    _isViewPortSizeBound = isBound;
}

void FBO::resize(int width, int height)
{
    // If the size is the same, don't do anything
    if(width == _originalSize[0] && height == _originalSize[1])
    {
        return;
    }
    // If the new size doesn't make sense, don't do anything
    if(width <= 0 || height <= 0)
    {
        return;
    }

    // Gather all attachments
    std::vector<ColorAttachment> colorAttachments = _colorAttachments;
    Attachment depthAttachment = _depthAttachment;
    Attachment stencilAttachment = _stencilAttachment;

    std::array<E_AttachmentTypes, 3> framebufferTemplate = _framebufferTemplate;
    // Reset the FBO
    init(framebufferTemplate);

    // Change the original size
    _originalSize = {static_cast<unsigned int>(width), static_cast<unsigned int>(height)};

    // Re-add all attachments
    for(auto attachment : colorAttachments)
    {
        addAttachment(E_AttachmentSlot::COLOR, attachment.format);
    }

    if(depthAttachment.id != -1)
    {
        addAttachment(E_AttachmentSlot::DEPTH);
    }

    if(stencilAttachment.id != -1)
    {
        addAttachment(E_AttachmentSlot::STENCIL);
    }
}
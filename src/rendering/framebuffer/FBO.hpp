#pragma once

// GLFW incldues
#include "rendering/GLFW_Wrapper.hpp"

// STL includes
#include <array>
#include <vector>

// First-party includes
#include "resources/Texture.hpp"

enum class E_ColorFormat
{
    RGB,                // 8 bits per channel, fixed point
    RGBA,               // 8 bits per channel, fixed point, alpha
    RGBA16F             // 16 bits per channel, floating point, alpha
};

enum class E_AttachmentSlot
{
    COLOR,
    DEPTH,
    STENCIL
};

enum class E_AttachmentTemplate
{   
    TEXTURE,                // Color = Texture  | Depth = Texture       | Stencil = Texture
    RENDERBUFFER,           // Color = Texture  | Depth = Renderbuffer  | Stencil = Renderbuffer
    SHADOW_DEPTH,           // Color = None     | Depth = Texture       | Stencil = None
    SHADOW_DEPTH_CUBE       // Color = None     | Depth = Cubemap       | Stencil = None
};

enum class E_AttachmentTypes
{
    NONE,
    TEXTURE,
    RENDERBUFFER,
    CUBEMAP
};


// Holds information about a depth & buffer attachments
struct Attachment
{
    unsigned int id;
    E_AttachmentTypes type;
};

// Extends information about a color attachment
struct ColorAttachment : public Attachment
{
    unsigned int slot;
    E_ColorFormat format;
};

class FBO
{
public:
    //FBO(const std::array<E_AttachmentTypes, 3>& templateTypes, unsigned int width, unsigned int height);
    FBO(E_AttachmentTemplate format, unsigned int width, unsigned int height);
    ~FBO();

    unsigned int id() const { return _id; }
    void reset();
    const std::array<unsigned int, 2>& getOriginalSize() const { return _originalSize; }

    void addAttachment(E_AttachmentSlot type, E_ColorFormat colorFormat = E_ColorFormat::RGB);

    const std::vector<ColorAttachment>& getColorAttachments() const { return _colorAttachments; }
    unsigned int getColorAttachmentID(unsigned int index) const { return _colorAttachments[index].id; }
    unsigned int getDepthAttachmentID() const { return _depthAttachment.id; }
    unsigned int getStencilAttachmentID() const { return _stencilAttachment.id; }

    std::vector<Texture> getTextures() const;
    unsigned int getDepthTextureID() const;

    void bindToViewport(bool isBound = true);

    void resize(unsigned int width, unsigned int height);

private:
    void init(const std::array<E_AttachmentTypes, 3>& templateTypes);

    ColorAttachment addColorAttachment(E_ColorFormat colorFormat = E_ColorFormat::RGB);
    Attachment addDepthAttachment(E_AttachmentTypes type);
    Attachment addStencilAttachment(E_AttachmentTypes type);

    unsigned int _id;
    std::array<unsigned int, 2> _originalSize;
    std::array<E_AttachmentTypes, 3> _framebufferTemplate;

    std::vector<ColorAttachment> _colorAttachments;
    Attachment _depthAttachment;
    Attachment _stencilAttachment;
    bool _isViewPortBound;
};

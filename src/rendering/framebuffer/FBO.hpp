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
    RGB,              // 8 bits per channel, fixed point
    RGBA,             // 8 bits per channel, fixed point, alpha
    RGBA16F        // 16 bits per channel, floating point, alpha
};

enum class E_AttachmentType
{
    COLOR,
    DEPTH,
    STENCIL
};

enum class E_AttachmentFormat
{
    TEXTURE,                // Color = Texture  | Depth = Texture       | Stencil = Texture
    RENDERBUFFER,           // Color = Texture  | Depth = Renderbuffer  | Stencil = Renderbuffer
    SHADOW_DEPTH,           // Color = None     | Depth = Texture       | Stencil = None
    SHADOW_DEPTH_CUBE       // Color = None     | Depth = Cubemap       | Stencil = None
};

struct ColorAttachment
{
    unsigned int id;
    unsigned int slot;
    E_ColorFormat format;
};

class FBO
{
public:
    FBO(unsigned int width, unsigned int height);
    virtual ~FBO() = 0{};

    unsigned int id() const { return _id; }
    const std::array<unsigned int, 2>& getOriginalSize() const { return _originalSize; }

    virtual void addAttachment(E_AttachmentType type, E_ColorFormat colorFormat = E_ColorFormat::RGB) = 0{};

    const std::vector<ColorAttachment>& getColorAttachments() const { return _colorAttachments; }
    unsigned int getColorAttachmentID(unsigned int index) const { return _colorAttachments[index].id; }
    unsigned int getDepthAttachmentID() const { return _depthAttachmentID; }
    unsigned int getStencilAttachmentID() const { return _stencilAttachmentID; }

    std::vector<Texture> getTextures() const;
    unsigned int getDepthTextureID() const;

protected:
    ColorAttachment addColorAttachment(E_ColorFormat colorFormat = E_ColorFormat::RGB);

    unsigned int _id;
    std::array<unsigned int, 2> _originalSize;

    std::vector<ColorAttachment> _colorAttachments;
    unsigned int _depthAttachmentID;
    unsigned int _stencilAttachmentID;
};

class TextureFBO : public FBO
{
public:
    TextureFBO(unsigned int width, unsigned int height);
    ~TextureFBO();

    void addAttachment(E_AttachmentType type, E_ColorFormat colorFormat = E_ColorFormat::RGB) override;

private:
    unsigned int addDepthAttachment();
    unsigned int addStencilAttachment();
};

class RenderBufferFBO : public FBO
{
public:
    RenderBufferFBO(unsigned int width, unsigned int height);
    ~RenderBufferFBO();

    void addAttachment(E_AttachmentType type, E_ColorFormat colorFormat = E_ColorFormat::RGB) override;

private:
    unsigned int addDepthAttachment();
    unsigned int addStencilAttachment();
};

class ShadowDepthFBO : public FBO
{
public:
    ShadowDepthFBO(unsigned int width, unsigned int height);
    ~ShadowDepthFBO();

    void addAttachment(E_AttachmentType type, E_ColorFormat colorFormat = E_ColorFormat::RGB) override;

private:
    unsigned int addDepthAttachment();
};


class ShadowDepthCubeFBO : public FBO
{
public:
    ShadowDepthCubeFBO(unsigned int width, unsigned int height);
    ~ShadowDepthCubeFBO();

    void addAttachment(E_AttachmentType type, E_ColorFormat colorFormat = E_ColorFormat::RGB) override;

private:
    unsigned int addDepthAttachment();
};


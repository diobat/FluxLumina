#pragma once

// GLFW incldues
#include "rendering/GLFW_Wrapper.hpp"

// STL includes
#include <array>
#include <vector>

// First-party includes
#include "resources/Texture.hpp"

enum E_AttachmentType
{
    COLOR,
    DEPTH,
    STENCIL
};

enum E_AttachmentFormat
{
    TEXTURE,                // Color = Texture  | Depth = Texture       | Stencil = Texture
    RENDERBUFFER,           // Color = Texture  | Depth = Renderbuffer  | Stencil = Renderbuffer
    SHADOW_DEPTH,           // Color = None     | Depth = Texture       | Stencil = None
    SHADOW_DEPTH_CUBE       // Color = None     | Depth = Cubemap       | Stencil = None
};

class FBO
{
public:
    FBO(unsigned int width, unsigned int height);
    virtual ~FBO() = 0{};

    unsigned int getID() const { return _id; }
    const std::array<unsigned int, 2>& getOriginalSize() const { return _originalSize; }

    virtual void addAttachment(E_AttachmentType type) = 0{};

    unsigned int getColorAttachmentID(unsigned int index) const { return _colorAttachmentIDs[index]; }
    unsigned int getDepthAttachmentID() const { return _depthAttachmentID; }
    unsigned int getStencilAttachmentID() const { return _stencilAttachmentID; }

    std::vector<Texture> getTextures() const;
    unsigned int getDepthTextureID() const;

protected:
    unsigned int addColorAttachment();

    unsigned int _id;
    std::array<unsigned int, 2> _originalSize;

    std::vector<unsigned int> _colorAttachmentIDs;
    unsigned int _depthAttachmentID;
    unsigned int _stencilAttachmentID;
};

class TextureFBO : public FBO
{
public:
    TextureFBO(unsigned int width, unsigned int height);
    ~TextureFBO();

    void addAttachment(E_AttachmentType type) override;

private:
    unsigned int addDepthAttachment();
    unsigned int addStencilAttachment();
};

class RenderBufferFBO : public FBO
{
public:
    RenderBufferFBO(unsigned int width, unsigned int height);
    ~RenderBufferFBO();

    void addAttachment(E_AttachmentType type) override;

private:
    unsigned int addDepthAttachment();
    unsigned int addStencilAttachment();
};

class ShadowDepthFBO : public FBO
{
public:
    ShadowDepthFBO(unsigned int width, unsigned int height);
    ~ShadowDepthFBO();

    void addAttachment(E_AttachmentType type) override;

private:
    unsigned int addDepthAttachment();
};


class ShadowDepthCubeFBO : public FBO
{
public:
    ShadowDepthCubeFBO(unsigned int width, unsigned int height);
    ~ShadowDepthCubeFBO();

    void addAttachment(E_AttachmentType type) override;

private:
    unsigned int addDepthAttachment();
};

#pragma once

// STL includes
#include <memory>

// STD library includes
#include <vector>

// First party includes
#include "rendering/framebuffer/FBO.h"

class FBOManager
{
public:
    FBOManager(GLFWwindow *window);
    ~FBOManager();

    std::shared_ptr<FBO> addFBO(E_AttachmentFormat format, int width = 0, int height = 0);
    void bindFBO(unsigned int fboIndex);
    void unbindFBO();

    unsigned int getFBOCount() const;

    std::shared_ptr<FBO> getFBO(unsigned int fboIndex) const;
    unsigned int getFBOIndex(std::shared_ptr<FBO> fbo) const;

    bool isFrameBufferComplete(std::shared_ptr<FBO>) const;
    bool isFrameBufferComplete(unsigned int fboIndex) const;

private:
    std::vector<std::shared_ptr<FBO>> _frameBufferObjects;
    int _currentFBOIndex;

    // GLFW window
    GLFWwindow *_window;
};

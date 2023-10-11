#pragma once

// STL includes
#include <memory>
#include <map>

// STD library includes
#include <vector>

// First party includes
#include "rendering/framebuffer/FBO.h"
#include "scene/Scene.h"

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

    bool bindSceneToFBO(std::shared_ptr<Scene> scene, std::shared_ptr<FBO> fbo);
    bool unbindSceneFromFBO(std::shared_ptr<Scene> scene);

    void bindProperFBOFromScene(std::shared_ptr<Scene> scene);

    bool isFrameBufferComplete(std::shared_ptr<FBO>) const;
    bool isFrameBufferComplete(unsigned int fboIndex) const;

private:
    std::vector<std::shared_ptr<FBO>> _frameBufferObjects;
    int _currentFBOIndex;

    std::map<std::shared_ptr<Scene>, std::shared_ptr<FBO>> _fboSceneMap;
    bool _forceDefault;

    // GLFW window
    GLFWwindow *_window;
};

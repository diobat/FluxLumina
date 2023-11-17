#pragma once

// STL includes
#include <memory>
#include <map>

// STD library includes
#include <vector>

// First party includes
#include "rendering/framebuffer/FBO.hpp"
#include "scene/Scene.hpp"

class GraphicalEngine;  

class FBOManager
{
public:
    FBOManager(GraphicalEngine *engine);
    ~FBOManager();

    std::shared_ptr<FBO> addFBO(E_AttachmentTemplate format,int width , int height);
    void removeFBO(std::shared_ptr<FBO> fbo);
    void bindFBO(unsigned int fboIndex);
    void bindFBO(std::shared_ptr<FBO> fbo);
    void unbindFBO();

    size_t getFBOCount() const;

    std::shared_ptr<FBO> getFBO(unsigned int fboIndex) const;
    std::shared_ptr<FBO> getSceneFBO(std::shared_ptr<Scene> scene) const;
    unsigned int getFBOIndex(std::shared_ptr<FBO> fbo) const;

    void parseNewScene(std::shared_ptr<Scene> scene);
    bool bindSceneToFBO(std::shared_ptr<Scene> scene, std::shared_ptr<FBO> fbo);
    bool unbindSceneFromFBO(std::shared_ptr<Scene> scene);

    void bindProperFBOFromScene(std::shared_ptr<Scene> scene);

    void clearAll();
    void clearColor();
    void clearDepth();
    void clearStencil();

    bool isFrameBufferComplete(std::shared_ptr<FBO>) const;
    bool isFrameBufferComplete(unsigned int fboIndex) const;

private:
    std::vector<std::shared_ptr<FBO>> _frameBufferObjects;
    int _currentFBOIndex;

    std::unordered_map<std::shared_ptr<Scene>, std::shared_ptr<FBO>> _fboSceneMap;
    bool _forceDefault;

    // The engine currently running this chain
    GraphicalEngine* _ranFrom;
};

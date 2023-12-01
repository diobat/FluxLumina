#include "util/TexManip.hpp"
#include "rendering/GLFW_Wrapper.hpp"

TextureManipulator::TextureManipulator() : 
    FB_A(0),
    FB_B(0)
{
    ;
}

TextureManipulator& TextureManipulator::Instance()
{
    if (instance == nullptr) 
    {
        instance = new TextureManipulator();

        glGenFramebuffers(1, &instance->FB_A);
        glGenFramebuffers(1, &instance->FB_B);
    } 
    return *instance;
}

void TextureManipulator::copyTexture(unsigned int src, unsigned int dst, int width, int height)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FB_A);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, src, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FB_B);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst, 0);

    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, FB_A);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FB_B);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}


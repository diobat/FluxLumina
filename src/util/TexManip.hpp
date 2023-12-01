#pragma once

#include <array>
#include <memory>

class TextureManipulator
{
public:
    static TextureManipulator& Instance();

    void copyTexture(unsigned int src, unsigned int dst, int width, int height);
    void eraseTexture(unsigned int tex, int width, int height, std::array<unsigned char, 4> color = {0, 0, 0, 0});
    
    unsigned int FB_A, FB_B;

private:
    TextureManipulator();
    TextureManipulator(const TextureManipulator&) = delete;
    TextureManipulator& operator=(const TextureManipulator&) = delete;

    inline static TextureManipulator* instance = nullptr;


};
#pragma once

namespace shapes
{
    namespace quad
    {
        unsigned int VAO();
    }

    namespace cube
    {
        unsigned int VAO();
    }

    namespace sphere
    {
        unsigned int VAO();
        unsigned int vertexCount();
        unsigned int indexCount();
    }
}
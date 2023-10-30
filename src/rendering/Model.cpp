#include "rendering\Model.hpp"


bool Model::hasTransparency() const
{
    for (auto& mesh : meshes)
    {
        if (mesh->_hasTransparency)
        {
            return true;
        }
    }
    return false;
}
#include "rendering\Model.h"


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
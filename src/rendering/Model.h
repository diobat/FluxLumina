/** 
 * Code taken from www.learnopengl.com
 * Slightly modified by Tomasz Gałaj 2018
 **/

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "Mesh.h"
#include "helpers/RootDir.h"

class Model
{
public:
    /*  Model Data */
    std::string directory;
    std::vector<Mesh> meshes;
};

#endif
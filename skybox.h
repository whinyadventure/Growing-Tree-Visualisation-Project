#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"

using namespace glm;

class Skybox {
private:
    const unsigned int vertex_count = 6;
    const float wall_size = 100.0;
    const float ground_size = 10;
    const float tile_size = 10.0;
public:
    Skybox() {}
    virtual ~Skybox() {}
    virtual void drawSolid(mat4, mat4, GLuint, GLuint, GLuint, GLuint, GLuint);
};

#endif


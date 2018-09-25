#ifndef LEAF_H
#define LEAF_H

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"

using namespace glm;
using namespace std;

class Leaf {
private:
    float max_size = MAX_LEAF;
    float angle = PI/3;
    float direction;
    float distance_from_basis;

    void setDirection();
    void setDistance();

public:
    Leaf();
    virtual ~Leaf();
    float getMax_size();
    float getAngle();
    float getDirection();
    float getDistance();
};

#endif

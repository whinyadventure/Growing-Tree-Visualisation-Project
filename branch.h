#ifndef BRANCH_H
#define BRANCH_H

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.h"

using namespace glm;
using namespace std;

class Branch {
private:
    Branch* parent;
    float max_length;
    float angle = 0.0;
    float direction;
    float distance_from_basis;
    int level;
    int children;
    bool is_trunk = false;

    void setAngle();
    void setDirection();
    void setDistance();
    void randChildren();

public:
    Branch(Branch*);
    virtual ~Branch();
    float getMaxLength();
    float getAngle();
    float getDirection();
    float getDistance();
    int getLvl();
    int getChildren();
    bool isTrunk();
};


#endif

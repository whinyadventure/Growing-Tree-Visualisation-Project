#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "constants.h"

using namespace std;
using namespace glm;

class Model_loader {
public:
    Model_loader() {}
    virtual ~Model_loader() {}
    bool loadObject(const char*, vector<float>&, vector<float>&, vector<float>&);
};

#endif

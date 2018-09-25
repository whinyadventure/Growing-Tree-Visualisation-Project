#ifndef TREE_H
#define TREE_H

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
#include "model_loader.h"
#include "branch.h"
#include "leaf.h"

using namespace glm;
using namespace std;

class Tree {
private:
    vector<float> vertices_branch, uvs_branch, normals_branch;
    vector<float> vertices_leaf, uvs_leaf, normals_leaf, colors_leaf;

    float *f_ptr_branch, *ft_ptr_branch, *n_ptr_branch;
    float *f_ptr_leaf, *ft_ptr_leaf, *n_ptr_leaf, *c_ptr_leaf;
    Branch *iterator;

    vector<Branch*> level_1, level_2, level_3;
    Leaf* leaves[20];
    Branch *trunk;

    void drawBranch(mat4, mat4, GLuint, GLuint, float, float, Branch*);
    void drawLeaf(mat4, mat4, GLuint, int, float);
public:
    Tree();
    virtual ~Tree(){}
    void loadModels();
    void draw(mat4, mat4, GLuint, GLuint, float);
};

#endif

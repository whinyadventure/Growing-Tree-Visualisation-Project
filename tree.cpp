#include "tree.h"

Tree::Tree(){
    trunk = new Branch(nullptr);

    // iterators & ptr
    Branch *iterator;
    int i;
    Leaf *leaf_ptr;

    for(i = 0; i < trunk->getChildren(); i++)
        level_1.push_back(new Branch(trunk));

    for(iterator: level_1){
        for(i = 0; i < (iterator->getChildren()); i++)
            level_2.push_back(new Branch(iterator));
    }

    for(iterator: level_2){
        for(i = 0; i < (iterator->getChildren()); i++)
            level_3.push_back(new Branch(iterator));
    }

    for(i = 0; i < 20; i++){
        leaf_ptr = new Leaf();
        leaves[i] = leaf_ptr;
    }

}

void Tree::loadModels(){
    Model_loader loader;
    loader.loadObject("branch_model.obj", vertices_branch, uvs_branch, normals_branch);

    loader.loadObject("leaf_model.obj", vertices_leaf, uvs_leaf, normals_leaf);
    for(int i = 0; i < vertices_leaf.size(); i++)
        colors_leaf.push_back(1);

    // branch x coordinate
    f_ptr_branch = &vertices_branch[0];
    ft_ptr_branch = &uvs_branch[0];
    n_ptr_branch = &normals_branch[0];

    // leaf x coordinate
    f_ptr_leaf = &vertices_leaf[0];
    ft_ptr_leaf = &uvs_leaf[0];
    n_ptr_leaf = &normals_leaf[0];
    c_ptr_leaf = &colors_leaf[0];
}

void Tree::drawLeaf(mat4 V, mat4 M, GLuint leaf_tex, int i, float time_counter){
        float length_temp = MAX_LEAF*time_counter;

        if(length_temp > MAX_LEAF) // if exceeded max size, then stop growing
            length_temp = MAX_LEAF;

        float ratio = length_temp/MAX_LEAF;

        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glBindTexture(GL_TEXTURE_2D, leaf_tex);

        glVertexPointer(3, GL_FLOAT, 0, f_ptr_leaf);
        glTexCoordPointer(2, GL_FLOAT, 0, ft_ptr_leaf);
        glNormalPointer(GL_FLOAT, 0, n_ptr_leaf);
        glColorPointer(3, GL_FLOAT, 0, c_ptr_leaf);

        M = translate(M, vec3(0.15f, leaves[i]->getDistance()*ratio*0.8, 0.0f));
        M = rotate(M, leaves[i]->getAngle()*(PI/180), vec3(0.0f, 1.0f, 0.0f));
        M = rotate(M, leaves[i]->getDirection(), vec3(1.0f, 0.0f, 0.0f));
        M = scale(M, vec3(length_temp, length_temp, length_temp));

        glLoadMatrixf(value_ptr(V*M));
        glDrawArrays(GL_TRIANGLES,0,vertices_leaf.size()/3);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisable(GL_TEXTURE_2D);
}

void Tree::drawBranch(mat4 V, mat4 M, GLuint bark_tex, GLuint leaf_tex, float time_counter, float scale_children, Branch *branch_ptr){
    int i;
    float length_temp = branch_ptr->getMaxLength()*time_counter;

    if(length_temp > branch_ptr->getMaxLength()){
        length_temp = branch_ptr->getMaxLength();
    }

    float ratio = length_temp/branch_ptr->getMaxLength();

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBindTexture(GL_TEXTURE_2D, bark_tex);

    glVertexPointer(3, GL_FLOAT, 0, f_ptr_branch);
    glTexCoordPointer(2, GL_FLOAT, 0, ft_ptr_branch);
    glNormalPointer(GL_FLOAT, 0, n_ptr_branch);

    if(branch_ptr->isTrunk())
        M = scale(M, vec3(ratio*X_Z_COEFF, ratio, ratio*X_Z_COEFF));

    if(!branch_ptr->isTrunk()){
        M = scale(M, vec3(ratio*X_Z_COEFF*scale_children, ratio*scale_children, ratio*X_Z_COEFF*scale_children));
        M = translate(M, vec3(0.0f, branch_ptr->getDistance()*ratio, 0.0f));
        M = rotate(M, branch_ptr->getDirection(),vec3(0.0f, 1.0f, 0.0f));
        M = rotate(M, branch_ptr->getAngle(), vec3(0.0f, 0.0f, 1.0f));
    }

    glLoadMatrixf(value_ptr(V*M));
    glDrawArrays(GL_TRIANGLES, 0, vertices_branch.size()/3);

    if(branch_ptr->getLvl() == 2 || branch_ptr->getLvl() == 3){ // draw leaves only on levels 2 and 3
        for(int i = 0; i < 20; i++)
            drawLeaf(V, M, leaf_tex, i, time_counter);

    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_TEXTURE_2D);

    for(i = 0; i < branch_ptr->getChildren(); i++){
        if(branch_ptr->getLvl() == 1){
            drawBranch(V, M, bark_tex, leaf_tex, time_counter, 0.8, level_2[i]);
        } else if(branch_ptr->getLvl() == 2){
            drawBranch(V, M, bark_tex, leaf_tex, time_counter, 0.9, level_3[i]);
        } else {
            return;
        }
    }
}

void Tree::draw(mat4 V, mat4 M, GLuint bark_tex, GLuint leaf_tex, float time_counter){
    drawBranch(V, M, bark_tex, leaf_tex, time_counter, 1.0, trunk);

    for(iterator: level_1)
        drawBranch(V, M, bark_tex, leaf_tex, time_counter, 0.5, iterator);
}

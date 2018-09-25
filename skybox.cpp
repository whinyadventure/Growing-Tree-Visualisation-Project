#include "skybox.h"

void Skybox::drawSolid(mat4 V,mat4 M, GLuint down_tex, GLuint left_tex, GLuint right_tex, GLuint front_tex, GLuint back_tex){

    // side walls vertices
    float vertices_sky[] = {
        wall_size/2, wall_size, 0.0f,
        -wall_size/2, wall_size, 0.0f,
        -wall_size/2, 0.0f, 0.0f,
        wall_size/2, 0.0f, 0.0f
    };

    // ground vertices
    float vertices_ground[] = {
        ground_size/2, ground_size, 0.0f,
        -ground_size/2, ground_size, 0.0f,
        -ground_size/2, 0.0f, 0.0f,
        ground_size/2, 0.0f, 0.0f
    };

    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    float vertexNormals[] = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0
    };

    // draw model
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // left wall
    glBindTexture(GL_TEXTURE_2D, left_tex);

    glVertexPointer(3, GL_FLOAT, 0, vertices_sky);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glNormalPointer(GL_FLOAT, 0, vertexNormals);

    M = translate(M, vec3(0.0f, 0.0f, wall_size/2));

    glLoadMatrixf(value_ptr(V*M));
    glDrawArrays(GL_QUADS, 0.0f, vertex_count);

    // right wall
    glBindTexture(GL_TEXTURE_2D, right_tex);

    M = translate(M, vec3(0.0f, 0.0f, -wall_size));
    M = rotate(M, PI, vec3(0.0f, 1.0f, 0.0f));

    glLoadMatrixf(value_ptr(V*M));
    glDrawArrays(GL_QUADS, 0, vertex_count);

    // back wall
    glBindTexture(GL_TEXTURE_2D, back_tex);

    M = rotate(M, -PI/2, vec3(0.0f, 1.0f, 0.0f));
    M = translate(M, vec3(-wall_size/2, 0.0f, wall_size/2));

    glLoadMatrixf(value_ptr(V*M));
    glDrawArrays(GL_QUADS, 0, vertex_count);

    // front wall
    glBindTexture(GL_TEXTURE_2D, front_tex);

    M = translate(M, vec3(0.0f, 0.0f, -wall_size));
    M = rotate(M, PI, vec3(0.0f, 1.0f, 0.0f));

    glLoadMatrixf(value_ptr(V*M));
    glDrawArrays(GL_QUADS, 0, vertex_count);

    // ground wall
    glVertexPointer(3, GL_FLOAT, 0, vertices_ground);
    glBindTexture(GL_TEXTURE_2D, down_tex);

    M = translate(M, vec3(-wall_size/2+ground_size/2, 0.0f, 0.0f));
    M = rotate(M, -PI/2, vec3(1.0f, 0.0f, 0.0f));

    glLoadMatrixf(value_ptr(V*M));
    glDrawArrays(GL_QUADS, 0, vertex_count);

    for(int i = 0; i < tile_size; i++){
        for(int j = 0; j < tile_size; j++){
            glLoadMatrixf(value_ptr(V*M));
            glDrawArrays(GL_QUADS, 0, vertex_count);

            M = translate(M, vec3(0.0f, ground_size, 0.0f));
        }
        M = translate(M, vec3(ground_size, -(ground_size*tile_size), 0.0f));
    }

    // clean after finished
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


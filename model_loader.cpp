#include "Model_loader.h"

bool Model_loader::loadObject(const char* path, vector<float>& out_vertices, vector<float>& out_uvs, vector<float>& out_normals){

    // Temporary variables
    vector<unsigned int> vertex_indices, uv_indices, normal_indices;
    vector<vec3> temp_vertices;
    vector<vec2> temp_uvs;
    vector<vec3> temp_normals;

    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("Impossible to open the file !\n");
        return false;
    }

    while(true){ // Read the file until the end
        char line_header[128];
        int res = fscanf(file, "%s", line_header);
        if (res == EOF)
            break;

        if (strcmp(line_header, "v") == 0){ // vec3
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(line_header, "vt") == 0){ // vec2
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }
        else if (strcmp(line_header, "vn") == 0){ // normals
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        else if (strcmp(line_header, "f") == 0){
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

            if (matches != 9){
                 printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }

            vertex_indices.push_back(vertexIndex[0]);
            vertex_indices.push_back(vertexIndex[1]);
            vertex_indices.push_back(vertexIndex[2]);
            uv_indices    .push_back(uvIndex[0]);
            uv_indices    .push_back(uvIndex[1]);
            uv_indices    .push_back(uvIndex[2]);
            normal_indices.push_back(normalIndex[0]);
            normal_indices.push_back(normalIndex[1]);
            normal_indices.push_back(normalIndex[2]);
        }
    }

    // Indexing
    for(unsigned int i = 0; i < vertex_indices.size(); i++){
        unsigned int vertexIndex = vertex_indices[i];
        vec3 vertex = temp_vertices[vertexIndex-1];

        out_vertices.push_back(vertex.x);
        out_vertices.push_back(vertex.y);
        out_vertices.push_back(vertex.z);
    }

    for(unsigned int i = 0; i < uv_indices.size(); i++){
        unsigned int uvIndex = uv_indices[i];
        vec2 uv = temp_uvs[uvIndex-1];

        out_uvs.push_back(uv.x);
        out_uvs.push_back(-uv.y);
    }

    for(unsigned int i = 0; i < normal_indices.size(); i++){
        unsigned int normalIndex = normal_indices[i];
        glm::vec3 normal = temp_normals[normalIndex-1];

        out_normals.push_back(normal.x);
        out_normals.push_back(normal.y);
        out_normals.push_back(normal.z);
    }
}

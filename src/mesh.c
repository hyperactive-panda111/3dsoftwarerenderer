#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"
#include "array.h"
#include "upng.h"

#define MAX_NUM_MESHES 10
static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;


void load_mesh(char* obj_filename, char* png_filename, vect3_t scale, vect3_t translation, vect3_t rotation) {
    // Load the OBJ file to our mesh
load_mesh_obj_data(&meshes[mesh_count], obj_filename);
    // Load the PNG file  to the mesh texture
    load_mesh_png_data(&meshes[mesh_count], png_filename);
    // Initialize scale, translation, and rotation with the parameters
    meshes[mesh_count].scale = scale;
    meshes[mesh_count].translation = translation;
    meshes[mesh_count].rotation = rotation;
    
    mesh_count++;
}

int get_num_meshes(void) {
    return mesh_count;
}


mesh_t* get_mesh(int index) {
    return &meshes[index];
}

void free_meshes(void) {
    for (int i = 0; i < mesh_count; i++) {
    upng_free(meshes[i].texture);
    array_free(meshes[i].faces);
    array_free(meshes[i].vertices);
    }
}

 void load_mesh_obj_data(mesh_t* mesh, char* obj_filename) {
    //open the file
    FILE* fp = fopen(obj_filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file at %s", obj_filename);
        exit(2);
    }
 
    int MAX_LINE = 1024;
    char line[MAX_LINE];

    tex2_t* texcoords = NULL;
    vect3_t vect;

    while (fgets(line, MAX_LINE, fp)) {
        //skip empty lines and comments
        if (line[0] == '#' || line[0] == '\n') continue;

        if (line[0] == 'v' && line[1] == ' ') {
            //Vertex Information
             sscanf(line, "v %f %f %f", &vect.x, &vect.y, &vect.z);
            array_push(mesh->vertices, vect);
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            //Face Information
            int texture_indices[3];
            int vertex_indices[3];
            int normal_indices[3];
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );
            face_t face = {
                .a = vertex_indices[0] - 1,
                .b = vertex_indices[1] - 1,
                .c = vertex_indices[2] - 1,
                .a_uv = texcoords[texture_indices[0] - 1],
                .b_uv = texcoords[texture_indices[1] - 1], 
                .c_uv = texcoords[texture_indices[2] - 1],
                .color = 0xFFFFFFFF
            };
            array_push(mesh->faces, face);
        }
        else if (line[0] == 'v' && line[1] == 't') {
            //Texture Coordinate Information
            tex2_t texcoord;

            sscanf(line, "vt %f %f", &texcoord.u, &texcoord.v);
            array_push(texcoords, texcoord);
        }
    }
    array_free(texcoords);
    fclose(fp);
}


void load_mesh_png_data(mesh_t* mesh, char* png_filename) {
    upng_t* png_texture = upng_new_from_file(png_filename);
    if (png_texture != NULL) {
        upng_decode(png_texture);
        if (upng_get_error(png_texture) == UPNG_EOK) {
            mesh->texture = png_texture;
        }
    }
}

#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"
 
//Define struct for dynamic sized meshes, with array of vertices and faces
typedef struct {
    vect3_t* vertices;      // dynamic array of vertices
    face_t* faces;          // dynamic array of faces
    upng_t* texture;        // mesh PNG texture pointer
    vect3_t rotation;       // mesh rotation with x, y and z values
    vect3_t scale;          // mesh scale with x, y, and z values
    vect3_t translation;    // mesh translation with x, y and z values
} mesh_t;

void load_mesh(char* obj_filename, char* png_filename, vect3_t scale, vect3_t translation, vect3_t rotation);
void load_mesh_obj_data(mesh_t* mesh, char* obj_filename);
void load_mesh_png_data(mesh_t* mesh, char* png_filename);
int get_num_meshes(void);
mesh_t* get_mesh(int index);

void free_meshes(void);

#endif

#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"
#include "triangle.h"
#include "texture.h"

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLES 10

enum {
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE
};

typedef struct {
    vect3_t point;
    vect3_t normal;
} plane_t;

typedef struct {
    vect3_t vertices[MAX_NUM_POLY_VERTICES];
    tex2_t texcoords[MAX_NUM_POLY_VERTICES];
    int num_vertices;
} polygon_t;

void init_frustum_planes(float fovx, float fovy,float z_near, float z_far);
polygon_t create_polygon_from_triangle(vect3_t v0, vect3_t v1, vect3_t v2, tex2_t t0, tex2_t t1, tex2_t t2);
void clip_polygon(polygon_t* polygon);
void triangles_from_polygon(polygon_t* polygon, triangle_t* triangles, int* num_triangles);

#endif

#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vect4_t mat4_mul_vect4_project(mat4_t mat_proj, vect4_t v);
vect4_t mat4_mul_vect4(mat4_t m, vect4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);
mat4_t mat4_look_at(vect3_t eye, vect3_t target, vect3_t up);

#endif

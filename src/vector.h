#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vect2_t;

typedef struct {
    float x;
    float y;
    float z;
} vect3_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vect4_t;

// 2D Vector Functions
float vect2_length(vect2_t v);
vect2_t vect2_normal(vect2_t v);

vect2_t vect2_add(vect2_t v1, vect2_t v2);
vect2_t vect2_sub(vect2_t v1, vect2_t v2);
vect2_t vect2_mul(vect2_t v, float factor);
vect2_t vect2_div(vect2_t v, float factor);
vect2_t vect2_new(float x, float y);
float vect2_dot(vect2_t v1, vect2_t v2);
void vect2_normalize(vect2_t* v);

// 3D Vector Functions
float vect3_length(vect3_t v);

vect3_t vect3_add(vect3_t v1, vect3_t v2);
vect3_t vect3_sub(vect3_t v1, vect3_t v2);
vect3_t vect3_mul(vect3_t v, float factor);
vect3_t vect3_div(vect3_t v, float factor);
vect3_t vect3_cross(vect3_t v1, vect3_t v2);
vect3_t vect3_new(float x, float y, float z);
float vect3_dot(vect3_t v1, vect3_t v2);
void vect3_normalize(vect3_t* ref);
vect3_t vect3_clone(vect3_t* v);

vect3_t vect3_rotate_x(vect3_t v, float angle);
vect3_t vect3_rotate_y(vect3_t v, float angle);
vect3_t vect3_rotate_z(vect3_t v, float angle);

// 4D Vector Functions
vect4_t vect4_from_vect3(vect3_t v);
vect3_t vect3_from_vect4(vect4_t v);
vect2_t vect2_from_vect4(vect4_t v);
#endif

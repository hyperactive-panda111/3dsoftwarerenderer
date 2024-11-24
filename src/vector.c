#include <math.h>
#include "vector.h"

//Implementation of 2D Vector Functions
float vect2_length(vect2_t v) {
    return sqrt((v.x * v.x) + (v.y * v.y)); 
}

vect2_t vect2_add(vect2_t v1, vect2_t v2) {
    vect2_t vect;

    vect.x = v1.x + v2.x;
    vect.y = v1.y + v2.y;

    return vect;
}

vect2_t vect2_sub(vect2_t v1, vect2_t v2) {
    vect2_t vect;

    vect.x = v1.x - v2.x;
    vect.y = v1.y - v2.y;

    return vect;
}

vect2_t vect2_mul(vect2_t v, float factor) {
    vect2_t vect = {
        .x = factor * v.x,
        .y = factor * v.y
    };

    return vect;
}

vect2_t vect2_div(vect2_t v, float factor) {
    vect2_t vect = {
        .x = v.x / factor,
        .y = v.y / factor
    };

    return vect;
}

float vect2_dot(vect2_t v1, vect2_t v2) {
        return (v1.x * v2.x) + (v1.y * v2.y);
}

void vect2_normalize(vect2_t* v) {
    float length = sqrt(v->x * v->x + v->y * v->y);

    v->x /= length;
    v->y /= length;
}

vect2_t vect2_new(float x, float y) {
    vect2_t result = {
        .x = x,
        .y = y
    };
    return result;
}

//Implementation of 3D Vector Functions
float vect3_length(vect3_t v) {
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

vect3_t vect3_add(vect3_t v1, vect3_t v2) {
    vect3_t vect;

    vect.x = v1.x + v2.x;
    vect.y = v1.y + v2.y;
    vect.z = v1.z + v2.z;

    return vect;
}

vect3_t vect3_sub(vect3_t v1, vect3_t v2) {
    vect3_t vect;

    vect.x = v1.x - v2.x;
    vect.y = v1.y - v2.y;
    vect.z = v1.z - v2.z;

    return vect;
}

vect3_t vect3_mul(vect3_t v, float factor) {
    vect3_t vect = {
        .x = factor * v.x,
        .y = factor * v.y,
        .z = factor * v.z
    };
    return vect;
}

vect3_t vect3_div(vect3_t v, float factor) {
    vect3_t vect = {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z / factor
    };
    return vect;
}

vect3_t vect3_cross(vect3_t v1, vect3_t v2) {
    vect3_t vect = {
        .x = (v1.y * v2.z) - (v1.z * v2.y),
        .y = (v1.z * v2.x) - (v1.x * v2.z),
        .z = (v1.x * v2.y) - (v1.y * v2.x)
    };
    return vect;
}

float vect3_dot(vect3_t v1, vect3_t v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void vect3_normalize(vect3_t* v) {
    float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x /= length;
    v->y /= length;
    v->z /= length;
}

vect3_t vect3_rotate_x(vect3_t v, float angle) {
    vect3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}

vect3_t vect3_rotate_y(vect3_t v, float angle) {
    vect3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z *cos(angle)
    };
    return rotated_vector;
}

vect3_t vect3_rotate_z(vect3_t v, float angle) {
    vect3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };
    return rotated_vector;
}

vect3_t vect3_new(float x, float y, float z) {
    vect3_t result = {
        .x = x,
        .y = y,
        .z = z
    };
    return result;
}

vect3_t vect3_clone(vect3_t* v) {
    vect3_t result = {
        .x = v->x,
        .y = v->y,
        .z = v->z,
    };
    return result;
}

// Implementation of 4D Vector Functions
vect4_t vect4_from_vect3(vect3_t v) {
    vect4_t result = {
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 1.0
    };

    return result;
}

vect3_t vect3_from_vect4(vect4_t v) {
    vect3_t result = {
        .x = v.x,
        .y = v.y,
        .z = v.z
    };
    return result;
}

vect2_t vect2_from_vect4(vect4_t v) {
    vect2_t result = {
        .x = v.x,
        .y = v.y
    };
    return result;
}

#include <math.h>
#include "matrix.h"

mat4_t mat4_identity(void) {
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |
    mat4_t m = {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }};
    return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
    // | sx  0   0  0 |
    // |  0  sy  0  0 |
    // |  0  0   sz 0 |
    // |  0  0   0  1 |
    mat4_t m = mat4_identity();
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;

    return m;
}

vect4_t mat4_mul_vect4(mat4_t m, vect4_t v) {
    vect4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

    return result;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
    // | 1 0 0 tx |
    // | 0 1 0 ty |
    // | 0 0 1 tz |
    // | 0 0 0  1 |
    mat4_t m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;

    return m;
}

mat4_t mat4_make_rotation_x(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    mat4_t m = mat4_identity();
    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4_make_rotation_y(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    mat4_t m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;

    return m;
}

mat4_t mat4_make_rotation_z(float angle) {
    float c = cos(angle);
    float s = sin(angle);

    mat4_t m = mat4_identity();
    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;

    return m;
}


mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
    mat4_t m;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    
    return m;
}

 
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar) {
    // | (h/w) * 1/tan(fov/2)                0                   0                 0 |
    // |                    0     1/tan(fov/2)                   0                 0 |
    // |                    0               0           zf/(zf-zn)  (-zf*zn)/(zf-zn) |
    // |                    0               0                    1                 0 |

    mat4_t m = {{{ 0 }}};
    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0;

    return m;
}

vect4_t mat4_mul_vect4_project(mat4_t mat_proj, vect4_t v) {
    // multiply the projection matrix by our original vector
    vect4_t result = mat4_mul_vect4(mat_proj, v);

    // perform perspective divide with original z-value that is now stored in w
    if (result.w != 0.0) {
        result.x /= result.w;
        result.y /= result.w;
    }
    return result;
}


mat4_t mat4_look_at(vect3_t eye, vect3_t target, vect3_t up) {
    vect3_t z = vect3_sub(target, eye);    // foward (z) vector
    vect3_normalize(&z);
    vect3_t x = vect3_cross(up, z);       // right (x) vector
    vect3_normalize(&x);
    vect3_t y = vect3_cross(z, x);       // up (y) vector
                                                       
    // | x.x    x.y     -dot(x, eye)    |
    // | y.x    y.y     -dot(y, eye)    |
    // | z.x    z.y     -dot(z, eye)    |
    // |   0      0                1    |
    mat4_t view_matrix = {{
        { x.x,  x.y,    x.z,    -vect3_dot(x, eye) },
        { y.x,  y.y,    y.z,    -vect3_dot(y, eye) },
        { z.x,  z.y,    z.z,    -vect3_dot(z, eye) },
        {   0,     0,      0,              1 }
    }};
    return view_matrix;
}

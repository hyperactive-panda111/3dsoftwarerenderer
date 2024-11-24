#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct {
    vect3_t position;
    vect3_t direction;
    vect3_t foward_velocity;
    float yaw;
    float pitch;
} camera_t;

void init_camera(vect3_t position, vect3_t direction);

vect3_t get_camera_position(void);
vect3_t get_camera_direction(void);
vect3_t get_camera_forward_velocity(void);

void update_camera_position(vect3_t position);
void update_camera_direction(vect3_t direction);
void update_camera_forward_velocity(vect3_t foward_velocity);

float get_camera_yaw(void);
float get_camera_pitch(void);

void rotate_camera_yaw(float angle);
void rotate_camera_pitch(float angle);

vect3_t get_camera_lookat_target(void);

#endif

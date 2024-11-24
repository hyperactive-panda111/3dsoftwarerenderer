#include "camera.h"
#include "matrix.h"

static camera_t camera;

void init_camera(vect3_t position, vect3_t direction) {
    camera.position = position;
    camera.direction = direction;
    camera.foward_velocity = vect3_new(0, 0, 0);
    camera.yaw = 0.0;
    camera.pitch = 0.0;
}

vect3_t get_camera_position(void) {
    return camera.position;
}

vect3_t get_camera_direction(void) {
    return camera.direction;
}

vect3_t get_camera_forward_velocity(void) {
    return camera.foward_velocity;
}

float get_camera_yaw(void) {
    return camera.yaw;
}

float get_camera_pitch(void) {
    return camera.pitch;
}

void update_camera_position(vect3_t position) {
    camera.position = position;
}

void update_camera_direction(vect3_t direction) {
    camera.direction = direction;
}

void update_camera_forward_velocity(vect3_t forward_velocity) {
    camera.foward_velocity = forward_velocity;
}

void rotate_camera_yaw(float angle) {
    camera.yaw += angle;
}

void rotate_camera_pitch(float angle) {
    camera.pitch += angle;
}

    
vect3_t get_camera_lookat_target(void) {
    // Initialize the target looking at the positive z-axis
    vect3_t target = { 0, 0, 1 };
    
    mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw);
    mat4_t camera_pitch_rotation = mat4_make_rotation_x(camera.pitch);

    // Create camera rotation matrix based on yaw and pitch
    mat4_t camera_rotation = mat4_identity();
    camera_rotation = mat4_mul_mat4(camera_pitch_rotation, camera_rotation);
    camera_rotation = mat4_mul_mat4(camera_yaw_rotation, camera_rotation);

    // Update camera direction based on the rotation
    vect4_t camera_direction = mat4_mul_vect4(camera_rotation, vect4_from_vect3(target));
    camera.direction = vect3_from_vect4(camera_direction);

    // Offset the camera position in the direction where the camera is pointing at
    target = vect3_add(camera.position, camera.direction);

    return target;
}

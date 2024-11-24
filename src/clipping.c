#include <math.h>
#include "clipping.h"

#define NUM_PLANES 6
plane_t frustum_planes[NUM_PLANES];

/*
///////////////////////////////////////////////////////////////////////////////
// Frustum planes are defined by a point and a normal vector
///////////////////////////////////////////////////////////////////////////////
// Near plane   :  P=(0, 0, znear), N=(0, 0,  1)
// Far plane    :  P=(0, 0, zfar),  N=(0, 0, -1)
// Top plane    :  P=(0, 0, 0),     N=(0, -cos(fov/2), sin(fov/2))
// Bottom plane :  P=(0, 0, 0),     N=(0, cos(fov/2), sin(fov/2))
// Left plane   :  P=(0, 0, 0),     N=(cos(fov/2), 0, sin(fov/2))
// Right plane  :  P=(0, 0, 0),     N=(-cos(fov/2), 0, sin(fov/2))
///////////////////////////////////////////////////////////////////////////////
//
//           /|\
//         /  | | 
//       /\   | |
//     /      | |
//  P*|-->  <-|*|   ----> +z-axis
//     \      | |
//       \/   | |
//         \  | | 
//           \|/
//
///////////////////////////////////////////////////////////////////////////////
*/

void init_frustum_planes(float fovx, float fovy, float z_near, float z_far) {
	float cos_half_fovx = cos(fovx / 2);
	float sin_half_fovx = sin(fovx / 2);
	float cos_half_fovy = cos(fovy / 2);
	float sin_half_fovy = sin(fovy / 2);

	frustum_planes[LEFT_FRUSTUM_PLANE].point = vect3_new(0, 0, 0);
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fovx;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fovx;

	frustum_planes[RIGHT_FRUSTUM_PLANE].point = vect3_new(0, 0, 0);
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fovx;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fovx;

	frustum_planes[TOP_FRUSTUM_PLANE].point = vect3_new(0, 0, 0);
	frustum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fovy;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fovy;

	frustum_planes[BOTTOM_FRUSTUM_PLANE].point = vect3_new(0, 0, 0);
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fovy;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fovy;

	frustum_planes[NEAR_FRUSTUM_PLANE].point = vect3_new(0, 0, z_near);
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustum_planes[FAR_FRUSTUM_PLANE].point = vect3_new(0, 0, z_far);
	frustum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}


polygon_t create_polygon_from_triangle(vect3_t v0, vect3_t v1, vect3_t v2, tex2_t t0, tex2_t t1, tex2_t t2) {
    polygon_t polygon = {
        .vertices = { v0, v1, v2 },
        .texcoords = { t0, t1, t2 },
        .num_vertices = 3,
    };
    return polygon;
} 


void triangles_from_polygon(polygon_t* polygon, triangle_t* triangles, int* num_triangles) {
    for (int i = 0; i < polygon->num_vertices - 2; i++) {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = vect4_from_vect3(polygon->vertices[index0]);
        triangles[i].points[1] = vect4_from_vect3(polygon->vertices[index1]);
        triangles[i].points[2] = vect4_from_vect3(polygon->vertices[index2]);

        triangles[i].texcoords[0] = polygon->texcoords[index0];
        triangles[i].texcoords[1] = polygon->texcoords[index1];
        triangles[i].texcoords[2]= polygon->texcoords[index2];
    }

    *num_triangles = polygon->num_vertices - 2;
}

float float_lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void clip_polygon_against_plane(polygon_t* polygon, int plane) {
    vect3_t plane_point = frustum_planes[plane].point;
    vect3_t plane_normal = frustum_planes[plane].normal;

    // The array of inside vertices that will be part of the final polygon returned by parameter
    vect3_t inside_vertices[MAX_NUM_POLY_VERTICES];
    tex2_t inside_texcoords[MAX_NUM_POLY_VERTICES];
    int num_inside_vertices = 0;

    // Start with current vertex with the first polygon vertex and texture coordinate
    vect3_t* current_vertex = &polygon->vertices[0];
    tex2_t* current_texcoord = &polygon->texcoords[0];

    // Start the previous vertex with the last polygon vertex and texture coordinate
    vect3_t* previous_vertex = &polygon->vertices[polygon->num_vertices - 1];
    tex2_t* previous_texcoord = &polygon->texcoords[polygon->num_vertices - 1];

    // Start the current and previous dot product
    float current_dot = 0; 
    float previous_dot = vect3_dot(vect3_sub(*previous_vertex, plane_point), plane_normal);

    // Loop while the current vertex is different from the last vertex
    while (current_vertex != &polygon->vertices[polygon->num_vertices]) {
        current_dot = vect3_dot(vect3_sub(*current_vertex, plane_point), plane_normal);

        // If we changed from inside to outside or vice-versa
        if (current_dot * previous_dot < 0) {
            // Calculate the interpolation factor t
            float t = previous_dot / (previous_dot - current_dot);

            // Calculate the intersection point (I = Q1 + t(Q2 - Q1)
            vect3_t intersection_point = {
                .x = float_lerp(previous_vertex->x, current_vertex->x, t),
                .y = float_lerp(previous_vertex->y, current_vertex->y, t),
                .z = float_lerp(previous_vertex->z, current_vertex->z, t)
            };
                                                                                    //
            tex2_t interpolated_texcoord = {
                .u = float_lerp(previous_texcoord->u, current_texcoord->u, t),
                .v = float_lerp(previous_texcoord->v, current_texcoord->v, t)
            };

            // Add the intersection point to the inside_vertices array
            inside_vertices[num_inside_vertices] = vect3_clone(&intersection_point);
            inside_texcoords[num_inside_vertices] = tex2_clone(&interpolated_texcoord);
            num_inside_vertices++;
    }

    // If current_point is inside the plane
    if (current_dot > 0) {
        // Add vertex to list of inside vertices
        inside_vertices[num_inside_vertices] = vect3_clone(current_vertex);
        inside_texcoords[num_inside_vertices] = tex2_clone(current_texcoord);
        num_inside_vertices++;
    }

    previous_vertex = current_vertex;
    previous_dot = current_dot;
    previous_texcoord = current_texcoord;
    current_vertex++;
    current_texcoord++;
    }

    for (int i = 0; i < num_inside_vertices; i++) {
       polygon->vertices[i] = vect3_clone(&inside_vertices[i]);
       polygon->texcoords[i] = tex2_clone(&inside_texcoords[i]);
    }

    polygon->num_vertices = num_inside_vertices;
}

void clip_polygon(polygon_t* polygon) {
    clip_polygon_against_plane(polygon, LEFT_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, RIGHT_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, TOP_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, BOTTOM_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, NEAR_FRUSTUM_PLANE);
    clip_polygon_against_plane(polygon, FAR_FRUSTUM_PLANE);
}

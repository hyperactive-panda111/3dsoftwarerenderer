#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "array.h"
#include "vector.h"
#include "clipping.h"
#include "camera.h"
#include "mesh.h"
#include "triangle.h"
#include "display.h"
#include "upng.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"

#define N_POINTS (9*9*9)

// Array of triangles to render
#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render;

/*
enum cull_method {
    CULL_NONE,
    CULL_BACKFACE
} cull_method;

enum render_method {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE
} render_method;
*/
    
///////////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
////////////////////////////////////////////////////////////////////////////////////////

vect3_t cube_points[N_POINTS];
vect2_t projected_points[N_POINTS];

//Global variables
mat4_t world_matrix; 
mat4_t proj_matrix;
mat4_t view_matrix;

// Global variables for game loop
bool is_running = false;
int previous_frame_time = 0;
float delta_time = 0.0;

void setup(void) {
    // Initialize render mode and triangle culling method
    set_render_method(RENDER_WIRE);
    set_cull_method(CULL_BACKFACE);
    //render_method = RENDER_TEXTURED;
    //cull_method = CULL_BACKFACE;

    // Initialize the scene light direction
    init_light(vect3_new(0, 0, 1));

    // Initialize the perspective projection matrix
    float aspecty = get_window_height() / (float)get_window_width();
    float aspectx = get_window_width() / (float)get_window_height();
    float fovy = M_PI / 3.0;
    float fovx = atan(tan(fovy / 2) * aspectx) * 2.0;
    float znear = 1.0;
    float zfar = 20.0;
    proj_matrix = mat4_make_perspective(fovy, aspecty, znear, zfar);
   
    // Initialize the frustum planes with a point and normal
    init_frustum_planes(fovx, fovy,  znear, zfar);
    
    load_mesh("./assets/runway.obj", "./assets/runway.png", vect3_new(1, 1, 1), vect3_new(0, -1.5, +23), vect3_new(0, 0, 0));
    load_mesh("./assets/f22.obj", "./assets/f22.png", vect3_new(1, 1, 1), vect3_new(0, -1.3, +5), vect3_new(0, -M_PI/2, 0));
    load_mesh("./assets/efa.obj", "./assets/efa.png", vect3_new(1, 1, 1), vect3_new(-2, -1.3, +9), vect3_new(0, -M_PI/2, 0));
    load_mesh("./assets/f117.obj", "./assets/f117.png", vect3_new(1, 1, 1), vect3_new(+2, -1.3, +9), vect3_new(0, -M_PI/2, 0));
} 

void process_input(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	    switch(event.type) {
		    case SDL_QUIT:
			    is_running = false;
			    break;
		    case SDL_KEYDOWN:
			    if (event.key.keysym.sym == SDLK_ESCAPE) {
				    is_running = false;
                    break;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    set_render_method(RENDER_WIRE_VERTEX);
                    break;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    set_render_method(RENDER_WIRE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_3) {
                    set_render_method(RENDER_FILL_TRIANGLE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_4) {
                    set_render_method(RENDER_FILL_TRIANGLE_WIRE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_5) {
                    set_render_method(RENDER_TEXTURED);
                    break;
                }
                if (event.key.keysym.sym == SDLK_6) {
                    set_render_method(RENDER_TEXTURED_WIRE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_c) {
                    set_cull_method(CULL_BACKFACE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_x) {
                    set_cull_method(CULL_NONE);
                    break;
                }
                if (event.key.keysym.sym == SDLK_w) {
                    rotate_camera_pitch(3.0 * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_s) {
                    rotate_camera_pitch(-3.0 * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    rotate_camera_yaw(1.0 * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    rotate_camera_yaw(-1.0 * delta_time);
                    break;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    update_camera_forward_velocity(vect3_mul(get_camera_direction(), 5.0 * delta_time));
                    update_camera_position(vect3_add(get_camera_position(), get_camera_forward_velocity())); 
                    break;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    update_camera_forward_velocity(vect3_mul(get_camera_direction(), 5.0 * delta_time));
                    update_camera_position(vect3_sub(get_camera_position(), get_camera_forward_velocity()));
                    break;
                }
  			    break;
	    }
    }
}

/*
///////////////////////////////////////////////////////////////////////////////
// Process the graphics pipeline stages for all the mesh triangles
///////////////////////////////////////////////////////////////////////////////
// +-------------+
// | Model space |  <-- original mesh vertices
// +-------------+
// |   +-------------+
// `-> | World space |  <-- multiply by world matrix
//     +-------------+
//     |   +--------------+
//     `-> | Camera space |  <-- multiply by view matrix
//         +--------------+
//         |    +------------+
//         `--> |  Clipping  |  <-- clip against the six frustum planes
//              +------------+
//              |    +------------+
//              `--> | Projection |  <-- multiply by projection matrix
//                   +------------+
//                   |    +-------------+
//                   `--> | Image space |  <-- apply perspective divide
//                        +-------------+
//                        |    +--------------+
//                        `--> | Screen space |  <-- ready to render
//                             +--------------+
///////////////////////////////////////////////////////////////////////////////
*/
void process_graphics_pipeline_stages(mesh_t* mesh) {
        // Create a scale, rotation and translation matrix that will be used to multiply the mesh vertices
        mat4_t scale_matrix = mat4_make_scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
        mat4_t translation_matrix = mat4_make_translation(mesh->translation.x, mesh->translation.y, mesh->translation.z);
        mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh->rotation.x);
        mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh->rotation.y);
        mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh->rotation.z);
         
        // Initialize the target looking at the positive z-axis
        vect3_t target = get_camera_lookat_target();
        vect3_t up_direction = vect3_new(0, 1, 0);
        view_matrix = mat4_look_at(get_camera_position(), target, up_direction);

        //Loop all triangle faces of our mesh
        int num_faces = array_length(mesh->faces);
        
        for (int i = 0; i < num_faces; i++) {
            face_t mesh_face = mesh->faces[i];

            vect3_t face_vertices[3];

            face_vertices[0] = mesh->vertices[mesh_face.a];
            face_vertices[1] = mesh->vertices[mesh_face.b];
            face_vertices[2] = mesh->vertices[mesh_face.c];

            vect4_t transformed_vertices[3];

            //Loop all three vertices of the current face and apply appropriate transformations
            for (int j = 0; j < 3; j++) {
                vect4_t transformed_vertex = vect4_from_vect3(face_vertices[j]);

                // Create a World Matrix combining scale, rotation, and translation matrices(in order)
                world_matrix = mat4_identity();
                world_matrix = mat4_mul_mat4(scale_matrix, world_matrix); 
                world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
                world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix); 
                world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix); 
                world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

                //Multiply the world matrix by the original vector
                transformed_vertex = mat4_mul_vect4(world_matrix, transformed_vertex);

                // Multiply the view matrix by the vector to transform the scene to camera scene
                transformed_vertex = mat4_mul_vect4(view_matrix, transformed_vertex);
                // Save transformed vertex in the array of transformed vertices
                transformed_vertices[j] = transformed_vertex;
            }

            // Calculate the triangle face normal
            vect3_t face_normal = get_triangle_normal(transformed_vertices);

            // Backface culling test to see if the current face should be projected
            if (should_cull_backface()) {

                // Find the vector between a point in the triangle and the camera origin
                vect3_t camera_ray = vect3_sub(vect3_new(0, 0, 0), vect3_from_vect4(transformed_vertices[0]));

            //Calculate how aligned the camera ray is aligned is with face normal (using dotproduct)
            float dot_normal_camera = vect3_dot(face_normal, camera_ray);

                // Bypass the triangle that are looking away from camera
                if (dot_normal_camera < 0)
                    continue; 
            }

            // Create a polygon from the original transform
            polygon_t polygon = create_polygon_from_triangle(
                    vect3_from_vect4(transformed_vertices[0]),
                    vect3_from_vect4(transformed_vertices[1]),
                    vect3_from_vect4(transformed_vertices[2]),
                    mesh_face.a_uv,
                    mesh_face.b_uv,
                    mesh_face.c_uv
            );

            // Clip the polygon and return a new polygon with potential new vertices
            clip_polygon(&polygon);

            // After clipping, break resultant polygon into triangles
            triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLES];
            int num_triangles_after_clipping = 0;

            triangles_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);
            
            // Loops all the assembled triangles after clipping
            for (int t = 0; t < num_triangles_after_clipping; t++) {
                triangle_t triangle_after_clipping = triangles_after_clipping[t];

                vect4_t projected_points[3];

                // Loop all three vertices to perform projection
                for (int j = 0; j < 3; j++) {
                    // Project the current vertex
                    projected_points[j] = mat4_mul_vect4_project(proj_matrix,triangle_after_clipping.points[j]);

                    //Invert the y values to account for flipped screen y coordinate
                    projected_points[j].y *= -1;

                    // Scale and translate the projected points to the middle of the screen
                    projected_points[j].x *= (get_window_width() / 2.0);
                    projected_points[j].y *= (get_window_height() / 2.0);

                    projected_points[j].x += (get_window_width() / 2.0);
                    projected_points[j].y += (get_window_height() / 2.0);
                }

                // Calculate the shade intensity value based on the inverse of the light angle in relation to the face normal
                float light_intesity_factor = -vect3_dot(face_normal, get_light_direction());

                // Calculate the triangle color based on the light angle
                uint32_t triangle_color = light_apply_intensity(mesh_face.color, light_intesity_factor);

                triangle_t triangle_to_render = {
                    .points = {
                        { projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
                        { projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
                        { projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w }
                    },
                    .texcoords = {
                        { triangle_after_clipping.texcoords[0].u, triangle_after_clipping.texcoords[0].v },
                        { triangle_after_clipping.texcoords[1].u, triangle_after_clipping.texcoords[1].v },
                        { triangle_after_clipping.texcoords[2].u, triangle_after_clipping.texcoords[2].v }
                    },
                    .color = triangle_color,
                    .texture = mesh->texture
                };


                // Save the projected triangle in the array of triangles to render
                if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
                    triangles_to_render[num_triangles_to_render++] = triangle_to_render;
                }
        }
}

// Function updates geometric data frame by frame
void update(void) {
    //wait some time until the reach the target frame time in milliseconds
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    //Delay execution if we are running too fast
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);

    // Get a delta time factor converted to seconds to be used to update our game objects
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;
    previous_frame_time = SDL_GetTicks();

    //Initialize the counter of triangles to render for current frame
    num_triangles_to_render = 0;
    
    // Loop all the meshes in the mesh array
    for (int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++) {
        mesh_t* mesh = get_mesh(mesh_index);

        // Change the mesh scale, rotation and translation values per animation frame
        //mesh->rotation.x += 0.0 * delta_time;
        //mesh->rotation.y += 0.3 * delta_time;
        //mesh->rotation.z += 0.0 * delta_time;
        //mesh->translation.x += 1 * delta_time;
        //mesh->translation.z = 5.0;

        //mesh->scale.x += 0.5;
        //mesh->scale.y += 0.001;

        // Process the graphics pipeline stages for every mesh of our 3D scene
        process_graphics_pipeline_stages(mesh);
    }
}
    
void render(void) {
    // Clear all the arrays to get ready for the next frame
	clear_color_buffer(0xFF000000);
    clear_z_buffer();

	draw_grid();
    
    //loop over projected triangles and render them
    for (int i = 0; i < num_triangles_to_render; i++) {
        triangle_t triangle = triangles_to_render[i];

        //Draw filled triangle
        if (should_render_filled_triangles()) {
            draw_filled_triangle(
                    triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, //vertex A
                    triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, //vertex B
                    triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, //vertex C
                    triangle.color
                    );
        }
        
        // Draw textured triangle
        if (should_render_textured_triangles()) {
           draw_textured_triangle(
                    triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,
                    triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,
                    triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,
                   triangle.texture 
                    );
        }

        // Draw triangle wireframe
        if (should_render_wireframe()) {
            draw_triangle(
                    triangle.points[0].x, triangle.points[0].y,
                    triangle.points[1].x, triangle.points[1].y,
                    triangle.points[2].x, triangle.points[2].y,
                    0xFFFFFFFF
                    );
        }

        //Draw vertex points
        if (should_render_wire_vertex()) {
            draw_rectangle(3, 3, triangle.points[0].x - 3, triangle.points[0].y - 3, 0xFFFF0000);
            draw_rectangle(3, 3, triangle.points[1].x - 3, triangle.points[1].y - 3, 0xFFFF0000);
            draw_rectangle(3, 3, triangle.points[2].x - 3, triangle.points[2].y - 3, 0xFFFF0000);
        }
    }
   
	render_color_buffer();
    
}

// Free dynamically allocated memory
void free_resources(void) {
	destroy_window();
    free_meshes();
}

int main(int argc, char* argv[]) {
	is_running = initialize_window();
	
	setup();
    
	while (is_running) {
		process_input();
		update();
		render();
	}

    free_resources();

	return (0);
} 

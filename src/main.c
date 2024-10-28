#include "display.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "vector.h"

#define N_POINTS (9*9*9)

///////////////////////////////////////////////////////////////////////////////////////
// Declare an array of vectors/points
////////////////////////////////////////////////////////////////////////////////////////

vect3_t cube_points[N_POINTS];
vect2_t projected_points[N_POINTS];
float fov_factor = 128;

bool is_running = false;

void setup(void) {
	//Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_height * window_width);

	if (!color_buffer) {
		fprintf(stderr, "Malloc allocation error.\n");
		exit(1);
	}

	//Creating a SDL Texture to display the color buffer
	color_buffer_texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			window_width,
			window_height
			);
    
    int point_count = 0;
    // Start loading my array of vectors
    for (float x = -1; x <= 1; x += 0.25) {
        for (float y = -1; y <= 1; y += 0.25) {
            for (float z = -1; z <= -1; z += 0.25) {
                vect3_t new_point = {x, y, z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);


	switch(event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
	}
}

vect2_t project(vect3_t point) {
    vect2_t projected_point = {
        .x = (fov_factor * point.x),
        .y = (fov_factor * point.y),
    }; 

    return projected_point;
}

void update(void) {
    for (int i = 0; i < N_POINTS; i++) {
        vect3_t point = cube_points[i];

        //Project the current 3D point
        vect2_t projected_point = project(point);

        //Save the projected 2D vector in the array of projected points
        projected_points[i] = projected_point;
    }
}

void render(void) {
	draw_grid();
	//draw_rectangle(200, 500, 500, 500, 0xFFFFFF00);

    for (int i = 0; i < N_POINTS; i++) {
        vect2_t projected_point = projected_points[i];
        draw_rectangle(
                4,
                4,
                projected_point.x +  (window_width / 2), 
                projected_point.y +  (window_height / 2),
                0xFFFFFF00
        );
    }
    
	render_color_buffer();
	//clear_color_buffer(0xFFFFFF00);

	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
	is_running = initialize_window();
	
	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return (0);
} 

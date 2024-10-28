#include "display.h"
#include <stdio.h>
#include <stdlib.h>

SDL_Texture* color_buffer_texture = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
int window_height = 600;
int window_width = 800;


bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Use SDL to query what is the fullscreen width and height of display
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create a SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr,"Error creating SDL window.\n");
		return false;
	}

	// Create a SDL Renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return  true;
}



void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++)
		for (int x = 0; x < window_width; x++)
			color_buffer[(window_width * y) + x] = color;
}

void draw_grid(void) {
	for (int y = 0; y < window_height; y += 10)
		for (int x = 0; x < window_width; x += 10) {
			if (y % 100 == 0) 
				color_buffer[(window_width * y) + x] = 0xFFFFFF00;
			if (x % 100 == 0)
				color_buffer[(window_width * y) + x] = 0xFFFFFF00;
		}
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
        int pixel_height = y * window_width;
        color_buffer[(pixel_height + x)] = color;
    }
}

void draw_rectangle(int height, int width, int x, int y, uint32_t color) {
	int viewport_height = window_width * y;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				//color_buffer[((viewport_height + (i * window_width)) + x) + j] = color;
                draw_pixel(x + j, y + i, color);
			}
}




void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}




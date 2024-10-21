#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>


extern SDL_Texture* color_buffer_texture;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;

extern int window_height;
extern int window_width;

bool initialize_window(void);
void draw_grid(void);
void draw_rectangle(int height, int width, int x, int y, uint32_t color);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif

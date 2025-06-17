#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "graphics.h"

// ### CONSTANTS ### //
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const float FIELD_OF_VIEW;
extern const float NEAR;
extern const float FAR;
const float X_ROTATION_THETA = 0.01f;
const float Y_ROTATION_THETA = 0.01f;
const float Z_ROTATION_THETA = 0.01f;

// ### MAIN FUNCTION ###
int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"Pixel Buffer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        0
	);

	if (!window) {
		printf("SDL_CreateWIndow Error %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Renderer error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT
	);

	if (!texture) {
		printf("Texture error %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Main Loop
	int running = 1;
	SDL_Event event;
	Uint32 frame_start, frame_time;
	float fps;
	const Uint32 frame_delay = 16;  // Approx 60 FPS
	
	// Graphics variables
	Vec3 eye = { 0.0f, 0.0f, 12.0f };
	Vec3 center = { 0.0f, 0.0f, 0.0f };
	Vec3 up = { 0.0f, 1.0f, 0.0f };
	Camera cam = { eye, center, up };
	
	// Cube
	Vec3 origin = { 0.0f, 0.0f, 0.0f };
	float side_length = 5.0f;
	Cube cube = create_cube(origin, side_length);
	ColorRgb red = { 200, 0, 0, 255 };
	ColorRgb green = { 0, 200, 0, 255 };
	ColorRgb blue = { 0, 0, 200, 255 };
	
	// Tetrehedron
	Tetrahedron th = create_tetrahedron(origin, side_length);
	
	// Line
	
	// Rotation Matrices
	Matrix4 x_rotation_matrix = rotation_xaxis(X_ROTATION_THETA);
	Matrix4 y_rotation_matrix = rotation_yaxis(Y_ROTATION_THETA);
	Matrix4 z_rotation_matrix = rotation_zaxis(Z_ROTATION_THETA);

	while (running) {
		frame_start = SDL_GetTicks();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
		}

		// Lock the texture to get a pixel buffer
		void *pixels;
		// Pitch is the number of bytes per row of pixels
		int pitch;
		if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
			printf("Lock error %s\n", SDL_GetError());
			continue;
		}

		// Convert pixel array to an array of Uint32
		Uint32 *buf = (Uint32*)pixels;
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				Uint8 r = 0;
				Uint8 g = 0;
				Uint8 b = 0;
				Uint8 a = 255;
				// Pixel at coordinates (x, y)
				// (0, 0) at top left and (639, 479) at bottom right
				buf[y * (pitch / 4) + x] = (a << 24) | (r << 16) | (g << 8) | b;
			}
		}
		// Draw Objects
		// POTENTIAL FIX: Clipping...
		// Cube
		bool cube_draw_result = draw_cube(buf, &cam, cube, green, pitch);
		if (!cube_draw_result) {
			printf("Error drawing cube\n");
		}
		// Tetrahedron
		bool th_draw_result = draw_tetrahedron(buf, &cam, th, red, pitch);
		if (!th_draw_result) {
			printf("Error drawing tetrahedron\n");
		}
		// Draw axis of rotation line
		bool line_draw_result = draw_line(buf, &cam, cube.vertices[0], cube.vertices[6], blue, pitch);
		if (!line_draw_result) {
			printf("Error drawing line\n");
		}
		
		// Update Objects
		// Rotate Cube Vertices along x-axis
		for (int i = 0; i < 8; i++) {
			Vec4 current_vertix = vec3_homogenous(cube.vertices[i], 1.0f);
			current_vertix = mat4_vec4_mul(x_rotation_matrix, current_vertix);
			cube.vertices[i] = (Vec3){ current_vertix.x, current_vertix.y, current_vertix.z };
		}
		// Rotate Cube Vertices along y-axis
		for (int i = 0; i < 8; i++) {
			Vec4 current_vertix = vec3_homogenous(cube.vertices[i], 1.0f);
			current_vertix = mat4_vec4_mul(y_rotation_matrix, current_vertix);
			cube.vertices[i] = (Vec3){ current_vertix.x, current_vertix.y, current_vertix.z };
		}
		// Rotate Cube Vertices along z-axis
		for (int i = 0; i < 8; i++) {
			Vec4 current_vertix = vec3_homogenous(cube.vertices[i], 1.0f);
			current_vertix = mat4_vec4_mul(z_rotation_matrix, current_vertix);
			cube.vertices[i] = (Vec3){ current_vertix.x, current_vertix.y, current_vertix.z };
		}
		
		SDL_UnlockTexture(texture);
		
		frame_time = SDL_GetTicks() - frame_start;
		fps = frame_time > 0 ? 1000.0f / frame_time : 0.0f;

		if (frame_time < frame_delay) {
			SDL_Delay(frame_delay - frame_time);
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}    

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
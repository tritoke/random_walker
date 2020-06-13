#include <arpa/inet.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>
#include "cmap.h"

#define WIDTH 1920
#define HEIGHT 1080
#define MAX(X, Y) (((X) < (Y)) ? (X) : (Y))

struct header {
	char magic[8];
	uint32_t width;
	uint32_t height;
};

struct canvas {
	uint32_t width;
	uint32_t height;
	Pixel ** pixels;
};

enum direction {
	Up,
	Down,
	Left,
	Right,
};

void random_walker(struct canvas *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, struct colourmap *);
struct canvas * make_canvas(uint32_t, uint32_t, Pixel);
void write_canvas(char *, struct canvas *);
void free_canvas(struct canvas *);

int main (void) {
	srand(time(NULL));

	struct colourmap * cm = read_map("../libcmap/colourmaps/plasma.cmap");

	Pixel white = {
	  .red = UINT16_MAX,
	  .blue = UINT16_MAX,
	  .green = UINT16_MAX,
	  .alpha = UINT16_MAX,
	};
	Pixel black = {
	  .alpha = UINT16_MAX,
	};

	struct canvas * canvas = make_canvas(WIDTH, HEIGHT, black);

	uint32_t borderpx = 20;
	for (uint32_t i = 0; i < 100000; i++) {
		random_walker(
			canvas,
			WIDTH / 2,
			HEIGHT / 2,
			6,
			10000,
			borderpx,
			cm
		);
	}

	write_canvas("walk.ff", canvas);

	free_canvas(canvas);
	free_cmap(cm);
	return 0;
}

void random_walker(struct canvas * canvas, uint32_t start_x, uint32_t start_y, uint32_t steps_per_thingamy,
	                 uint32_t num_steps, uint32_t borderpx, struct colourmap * cm) {
	assert(start_x > borderpx);
	assert(start_x < WIDTH - borderpx);
	assert(start_y > borderpx);
	assert(start_y < HEIGHT - borderpx);

	uint32_t steps_taken = 0;
	enum direction prev_direction = Up,
	               direction = (enum direction) rand() % 4;

	uint32_t x = start_x;
	uint32_t y = start_y;

	Pixel colour = cm->colours[0];
	memcpy(&canvas->pixels[y][x], &colour, sizeof colour);

	do {
		direction = rand() % 4;
							 
		if ((direction == Up && prev_direction == Down)
		 || (direction == Down && prev_direction == Up)
		 || (direction == Left && prev_direction == Right)
		 || (direction == Right && prev_direction == Left)
		) continue;
		if ((direction == Up && y < borderpx)
		 || (direction == Down && y > canvas->height - steps_per_thingamy - borderpx)
		 || (direction == Left && x < borderpx)
		 || (direction == Right && x > canvas->width - steps_per_thingamy - borderpx)
		) break;

		//if (direction == Left) continue;

		for (uint32_t i = 0; i < steps_per_thingamy; i++) {
			switch (direction) {
				case Up:
					y--;
					break;
				case Down:
					y++;
					break;
				case Left:
					x--;
					break;
				case Right:
					x++;
					break;
				default:
					fprintf(stderr, "Got unidentified direction: %d\n", (int) direction);
					exit(EXIT_FAILURE);
			}

			colour = cm->colours[steps_taken * cm->size / num_steps];
		}

		prev_direction = direction;
		steps_taken++;
	} while (steps_taken < num_steps);/* && x < canvas->width - borderpx - 1);*/
}

struct canvas * make_canvas(uint32_t width, uint32_t height, Pixel bg) {
	struct canvas * canvas = calloc(1, sizeof(struct canvas));

	canvas->width = width;
	canvas->height = height;
	canvas->pixels = malloc(height * sizeof(Pixel *));

	for (uint32_t y = 0; y < canvas->height; y++) {
		canvas->pixels[y] = malloc(canvas->width * sizeof(Pixel));
	}

	/* memcpy first row */
	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			canvas->pixels[y][x] = bg;
		}
	}

	return canvas;
}

void write_canvas(char * fname, struct canvas * canvas) {
	struct header header = {
		.magic = "farbfeld",
		.width = htonl(canvas->width),
		.height = htonl(canvas->height),
	};

	FILE * fp = fopen(fname, "w");

	fwrite(&header, sizeof header, 1, fp);
	for (uint32_t y = 0; y < canvas->height; y++) {
		fwrite(canvas->pixels[y], sizeof(Pixel), canvas->width, fp);
	}

	fclose(fp);
}

void free_canvas(struct canvas * canvas) {
	for (uint32_t y = 0; y < canvas->height; y++) {
		free(canvas->pixels[y]);
	}
	free(canvas->pixels);
	free(canvas);
}

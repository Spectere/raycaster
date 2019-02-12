/* palette.c */

#include <stdlib.h>
#include <sys/stat.h>

#include "../log.h"
#include "palette.h"

int palette_colors;
color_t *palette = palette_cga;

void palette_load(char* filename) {
	struct stat st;
	int colors, i = 0;
	FILE *pal_file;

	lprint(INFO, "Loading palette: %s", filename);

	if(lstat(filename, &st) == -1) {
		lprint(ERROR, "Could not stat file: %s", filename);
		return;
	}

	colors = (int)(st.st_size / 3);
	lprint(DEBUG, "File is %llu bytes long (%i colors)", (unsigned long long)st.st_size, colors);

	if(st.st_size % 3 != 0)
		lprint(WARN, "Palette file's size is not evenly divisible by 3. File may not load properly!");

	/* Load the file and put the data into memory. */
	palette = malloc(sizeof(color_t) * colors);
	pal_file = fopen(filename, "rb");
	if(pal_file == NULL) {
		lprint(ERROR, "Could not open file: %s", filename);
		return;
	}

	for(; i < colors; i++) {
		palette[i].a = 0xFF;
		palette[i].r = (Uint8)(fgetc(pal_file) * 4);
		palette[i].g = (Uint8)(fgetc(pal_file) * 4);
		palette[i].b = (Uint8)(fgetc(pal_file) * 4);
	}

	fclose(pal_file);
}

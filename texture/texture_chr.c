/* texture_chr.c */

#include <SDL.h>
#include <stdio.h>
#include <sys/stat.h>

#include "texture_chr.h"

SDL_bool texture_chr_check(FILE *f, struct stat *st) {
	return st->st_size % 8 == 0;
}

SDL_bool texture_chr_load(FILE *f) {
	return SDL_FALSE;
}

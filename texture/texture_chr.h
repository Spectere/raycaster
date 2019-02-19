/* texture_chr.h - A loader for 8x?? 1BPP character files (used by MegaZeux,
 *                 etc.*/

#ifndef _TEXTURE_CHR_H
#define _TEXTURE_CHR_H

SDL_bool texture_chr_check(FILE *f, struct stat *st);
SDL_bool texture_chr_load(FILE *f);

#endif /* _TEXTURE_CHR_H */

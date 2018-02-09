/*******************************************************************************
 * Stefan Bylund 2017
 *
 * Graphics utility functions.
 ******************************************************************************/

#ifndef _GFX_UTIL_H
#define _GFX_UTIL_H

#include <stdint.h>

/*
 * Load the given number of lines from the specified layer 2 screen file using
 * ESXDOS into the layer 2 screen memory.
 *
 * The layer 2 screen file contains the palette followed by the image. The
 * palette consists of 256 RGB333 colors and is 512 bytes long. The RGB333
 * colors are stored as an RGB332 byte followed by a zero-extended byte
 * containing the lowest blue bit. The image consists of pixel bytes in linear
 * order from left-to-right and top-to-bottom. The pixel bytes are indexes into
 * the 256 color palette.
 *
 * MMU slot 7 is temporarily used when loading the layer 2 screen file.
 * The layer 2 write-only paging area at the bottom 16 KB cannot be used when
 * using ESXDOS.
 *
 * If there is any error when loading the file, errno is set with the
 * corresponding ESXDOS error code.
 */
void gfx_load_screen(const char *filename, uint8_t num_lines);

#endif

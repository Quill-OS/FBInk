/*
 * fbtestfnt.c
 *
 * http://raspberrycompote.blogspot.com/2014/04/low-level-graphics-on-raspberry-pi-text.html
 *
 * Original work by J-P Rosti (a.k.a -rst- and 'Raspberry Compote')
 *
 * Licensed under the Creative Commons Attribution 3.0 Unported License
 * (http://creativecommons.org/licenses/by/3.0/deed.en_US)
 *
 * Distributed in the hope that this will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "eink/mxcfb-kobo.h"
#include "font8x8/font8x8_latin.h"
#include "fbink.h"

// default framebuffer palette
typedef enum
{
	BLACK        = 0,  /*   0,   0,   0 */
	BLUE         = 1,  /*   0,   0, 172 */
	GREEN        = 2,  /*   0, 172,   0 */
	CYAN         = 3,  /*   0, 172, 172 */
	RED          = 4,  /* 172,   0,   0 */
	PURPLE       = 5,  /* 172,   0, 172 */
	ORANGE       = 6,  /* 172,  84,   0 */
	LTGREY       = 7,  /* 172, 172, 172 */
	GREY         = 8,  /*  84,  84,  84 */
	LIGHT_BLUE   = 9,  /*  84,  84, 255 */
	LIGHT_GREEN  = 10, /*  84, 255,  84 */
	LIGHT_CYAN   = 11, /*  84, 255, 255 */
	LIGHT_RED    = 12, /* 255,  84,  84 */
	LIGHT_PURPLE = 13, /* 255,  84, 255 */
	YELLOW       = 14, /* 255, 255,  84 */
	WHITE        = 15  /* 255, 255, 255 */
} COLOR_INDEX_T;

static unsigned short def_r[] = { 0, 0, 0, 0, 172, 172, 172, 168, 84, 84, 84, 84, 255, 255, 255, 255 };
static unsigned short def_g[] = { 0, 0, 168, 168, 0, 0, 84, 168, 84, 84, 255, 255, 84, 84, 255, 255 };
static unsigned short def_b[] = { 0, 172, 0, 168, 0, 172, 0, 168, 84, 255, 84, 255, 84, 255, 84, 255 };

// 'global' variables to store screen info
char*                    fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

// helper function to 'plot' a pixel in given color
void
    put_pixel_Gray8(int x, int y, int c)
{
	// calculate the pixel's byte offset inside the buffer
	unsigned int pix_offset = x + y * finfo.line_length;

	// now this is about the same as 'fbp[pix_offset] = value'
	*((char*) (fbp + pix_offset)) = c;
}

void
    put_pixel_RGB24(int x, int y, int r, int g, int b)
{
	// remember to change main(): vinfo.bits_per_pixel = 24;
	// and: screensize = vinfo.xres * vinfo.yres *
	//                   vinfo.bits_per_pixel / 8;

	// calculate the pixel's byte offset inside the buffer
	// note: x * 3 as every pixel is 3 consecutive bytes
	unsigned int pix_offset = x * 3 + y * finfo.line_length;

	// now this is about the same as 'fbp[pix_offset] = value'
	*((char*) (fbp + pix_offset))     = b;
	*((char*) (fbp + pix_offset + 1)) = g;
	*((char*) (fbp + pix_offset + 2)) = r;
}

void
    put_pixel_RGB32(int x, int y, int r, int g, int b)
{
	// remember to change main(): vinfo.bits_per_pixel = 32;
	// and: screensize = vinfo.xres * vinfo.yres *
	//                   vinfo.bits_per_pixel / 8;

	// calculate the pixel's byte offset inside the buffer
	// note: x * 4 as every pixel is 4 consecutive bytes
	unsigned int pix_offset = x * 4 + y * finfo.line_length;

	// now this is about the same as 'fbp[pix_offset] = value'
	*((char*) (fbp + pix_offset))     = b;
	*((char*) (fbp + pix_offset + 1)) = g;
	*((char*) (fbp + pix_offset + 2)) = r;
	*((char*) (fbp + pix_offset + 3)) = 0xFF;    // Opaque, always.
}

void
    put_pixel_RGB565(int x, int y, int r, int g, int b)
{
	// remember to change main(): vinfo.bits_per_pixel = 16;
	// or on RPi just comment out the whole 'Change vinfo'
	// and: screensize = vinfo.xres * vinfo.yres *
	//                   vinfo.bits_per_pixel / 8;

	// calculate the pixel's byte offset inside the buffer
	// note: x * 2 as every pixel is 2 consecutive bytes
	unsigned int pix_offset = x * 2 + y * finfo.line_length;

	// now this is about the same as 'fbp[pix_offset] = value'
	// but a bit more complicated for RGB565
	unsigned short c = ((r / 8) << 11) + ((g / 4) << 5) + (b / 8);
	// or: c = ((r / 8) * 2048) + ((g / 4) * 32) + (b / 8);
	// write 'two bytes at once'
	*((unsigned short*) (fbp + pix_offset)) = c;
}

// handle various bpp...
void
    put_pixel(int x, int y, int c)
{
	if (vinfo.bits_per_pixel == 8) {
		put_pixel_Gray8(x, y, c);
	} else if (vinfo.bits_per_pixel == 16) {
		put_pixel_RGB565(x, y, def_r[c], def_g[c], def_b[c]);
	} else if (vinfo.bits_per_pixel == 24) {
		put_pixel_RGB24(x, y, def_r[c], def_g[c], def_b[c]);
	} else if (vinfo.bits_per_pixel == 32) {
		put_pixel_RGB32(x, y, def_r[c], def_g[c], def_b[c]);
	}
}

// helper function to draw a rectangle in given color
void
    fill_rect(int x, int y, int w, int h, int c)
{
	int cx, cy;
	for (cy = 0; cy < h; cy++) {
		for (cx = 0; cx < w; cx++) {
			put_pixel(x + cx, y + cy, c);
		}
	}
}

// helper function to clear the screen - fill whole
// screen with given color
void
    clear_screen(int c)
{
	memset(fbp, c, vinfo.xres * vinfo.yres);
}

// Render a specific font8x8 glyph into a 8x8 pixmap
char* font8x8_render(int ascii)
{
	// Get the bitmap for that ASCII character
	// TODO: Proper validation (w/ the right array depending on the range, to pickup non-basic stuff)
	if (ascii > 127 || ascii < 0) {
		// Default to space when OOR
		ascii = 0;
	}

	char *bitmap = font8x8_basic[ascii];

	int x, y = 0;
	bool set = false;
	// FIXME: static for now because I'm lazy. Move to let the caller alloc (on stack, obv.) & pass a pointer.
	static char glyph_pixmap[FONTW * FONTH] = { 0 };
	for (x=0; x < FONTW;  x++) {
		// x: input & output row
		for (y=0; y < FONTH; y++) {
			// y: input & output column
			set = bitmap[x] & 1 << y;
			// 'Flatten' our pixmap into a 1D array (0 = 0,0; 1=0,1; 2=0,2; 8=1,0)
			//int idx = x + (y * FONTH);	// 90° Left rotattion ;).
			int idx = y + (x * FONTW);
			printf("idx: %d @ x: %d & y: %d\n", idx, x, y);
			glyph_pixmap[idx] = set ? 1 : 0;
			//printf("%c", set ? 'X' : ' '); // y
		}
		//printf("\n");
	}

	return glyph_pixmap;
}

// Render a specific font8x8 glyph into a 16x16 pixmap
void font8x8_render_x2(int ascii, char* glyph_pixmap)
{
	// Get the bitmap for that ASCII character
	// TODO: Proper validation (w/ the right array depending on the range, to pickup non-basic stuff)
	if (ascii > 127 || ascii < 0) {
		// Default to space when OOR
		ascii = 0;
	}

	char *bitmap = font8x8_basic[ascii];

	int x, y, i, j = 0;
	bool set = false;
	for (i=0; i < FONTW*2;  i++) {
		// x: input row, i: output row
		x = i / 2;
		for (j=0; j < FONTH*2; j++) {
			// y: input column, j: output column
			y = j / 2;
			set = bitmap[x] & 1 << y;
			// 'Flatten' our pixmap into a 1D array (0 = 0,0; 1=0,1; 2=0,2; 8=1,0)
			int idx = j + (i * FONTW*2);
			printf("idx: %d @ x: %d & y: %d vs. i: %d & j: %d\n", idx, x, y, i, j);
			if (glyph_pixmap[idx] != 1)
				glyph_pixmap[idx] = set ? 1 : 0;
			//glyph_pixmap[idx+1] = set ? 1 : 0;
			idx = j + 1 + (i * FONTW*2);
			printf("idx: %d\n", idx);
			if (glyph_pixmap[idx] != 1)
				glyph_pixmap[idx] = set ? 1 : 0;
		}
	}
}

// helper function for drawing - no more need to go mess with
// the main function when just want to change what to draw...
void
    draw(char* arg)
{

	//fill_rect(0, 0, vinfo.xres, vinfo.yres, 1);

	char* text  = (arg != 0) ? arg : "Hello World!";
	int   textX = FONTW*2;
	int   textY = FONTH*2;
	int   textC = 0;
	int   bgC   = 15;

	// Row/Column offsets
	int row_off = 1;
	int col_off = 1;

	int i, l, x, y;

	// loop through all characters in the text string
	l = strlen(text);
	for (i = 0; i < l; i++) {
		// get the 'image' index for this character
		//int ix = font_index(text[i]);
		// get the font 'image'
		//char* img = fontImg[ix];
		//char* img = font8x8_render(text[i]);
		char img[FONTW*2 * FONTH*2] = { 0 };
		font8x8_render_x2(text[i], img);
		// loop through pixel rows
		for (y = 0; y < FONTH*2; y++) {
			// loop through pixel columns
			for (x = 0; x < FONTW*2; x++) {
				// get the pixel value
				char b = img[y * FONTW*2 + x];
				if (b > 0) {    // plot the pixel
					put_pixel(textX + (i * FONTW*2) + x + (row_off * FONTW*2),
						  textY + y + (col_off * FONTH*2),
						  textC);
				} else {
					put_pixel(textX + (i * FONTW*2) + x + (row_off * FONTW*2),
						  textY + y + (col_off * FONTH*2),
						  bgC);    // plot 'text backgr color'
				}
			}    // end "for x"
		}            // end "for y"
	}                    // end "for i"
}

// handle eink updates
void
    refresh(int fbfd)
{
	struct mxcfb_update_data update = {
		.temp                 = TEMP_USE_AMBIENT,
		.update_marker        = getpid(),
		.update_mode          = UPDATE_MODE_FULL,
		.update_region.height = vinfo.yres,
		.update_region.width  = vinfo.xres,
		.waveform_mode        = WAVEFORM_MODE_AUTO,
	};

	if (ioctl(fbfd, MXCFB_SEND_UPDATE, &update) < 0) {
		perror("MXCFB_SEND_UPDATE");
		exit(EXIT_FAILURE);
	}
}

// application entry point
int
    main(int argc, char* argv[])
{

	int                      fbfd = 0;
	struct fb_var_screeninfo orig_vinfo;
	long int                 screensize = 0;

	// Open the framebuffer file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		return (1);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
	}
	printf("Original %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
	}
	printf("Fixed info: smem_len %d, line_length %d\n", finfo.smem_len, finfo.line_length);

	// map fb to user mem
	screensize = finfo.smem_len;
	fbp        = (char*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if ((int) fbp == -1) {
		printf("Failed to mmap.\n");
	} else {
		// draw...
		draw(argv[1]);
	}

	// Refresh screen
	refresh(fbfd);

	// cleanup
	munmap(fbp, screensize);
	close(fbfd);

	return EXIT_SUCCESS;
}

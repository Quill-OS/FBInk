/*
   unibdf2hex - program to convert a BDF file into a unifont.hex file.

   Author: Paul Hardy, January 2008

   Copyright (C) 2008, 2013 Paul Hardy

   NOTE: I tweaked it to deal with the conversion of Cozette's BDF to HEX,
         which was all kinds of broken with gbdfed...

   gcc -Wall -Wextra tools/unibdf2hex.c -o tools/unibdf2hex

   LICENSE:

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 2 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define UNISTOP  0xFFFFFFu

#define MAXBUF 256

int
    main()
{
	int  i;
	int  digitsout; /* how many hex digits we output in a bitmap */
	uint32_t thispoint;
	uint32_t thishex;
	char inbuf[MAXBUF];
	int  bbxx, bbxy, bbxxoff, bbxyoff;

	int      descent = 3; /* font descent wrt baseline */
	int      startrow;    /* row to start glyph        */
	unsigned rowout;

	while (fgets(inbuf, MAXBUF - 1, stdin) != NULL) {
		// Cozette goes above U+FFFF, attempt to handle that...
		if (strncmp(inbuf, "STARTCHAR ", 10) == 0) {
			thishex = 0U;
			sscanf(&inbuf[10], "u%X", &thishex); /* get code point if in uFFFF format */
		}
		if (strncmp(inbuf, "ENCODING ", 9) == 0) {
			bool skip = false;
			sscanf(&inbuf[9], "%d", &thispoint); /* get code point (use %d because it may be -1!) */
			// If cp appears invalid, try to use the value from STARTCHAR...
			if (thispoint == UINT32_MAX && thishex != 0U) {
				thispoint = thishex;
			}
			if (thispoint <= UNISTOP) {
				/* If we want this code point, get the BBX (bounding box) and BITMAP information. */
				while (fgets(inbuf, MAXBUF - 1, stdin) != NULL && strncmp(inbuf, "BBX ", 4) != 0) {
					/* find bounding box */
					;
				}

				sscanf(&inbuf[4], "%d %d %d %d", &bbxx, &bbxy, &bbxxoff, &bbxyoff);

				// Skip glyph if it's too wide (> 8px)
				if (bbxx > 8) {
					fprintf(stderr, "Skipping U+%04X because it's too wide!\n", thispoint);
					skip = true;
				}

				while (fgets(inbuf, MAXBUF - 1, stdin) != NULL && strncmp(inbuf, "BITMAP", 6) != 0) {
					/* find bitmap start */
					;
				}
				if (!skip) {
					fprintf(stdout, "%04X:", thispoint);
				}
				digitsout = 0;
				/* Print initial blank rows */
				startrow = descent + bbxyoff + bbxy;
				// Recap metrics for debugging purposes...
				fprintf(stderr, "U+%04X metrics: %02dx%02d @ (%02d, %02d) => startrow: %02d\n", thispoint, bbxx, bbxy, bbxxoff, bbxyoff, startrow);

				/* Force everything to 13 pixels tall */
				if (!skip) {
					for (i = 13; i > startrow; i--) {
						fprintf(stdout, "00");
						digitsout += 2;
					}
				}
				while (fgets(inbuf, MAXBUF - 1, stdin) != NULL && strncmp(inbuf, "END", 3) != 0) {
					/* copy bitmap until END */
					sscanf(inbuf, "%X", &rowout);
					/* Now force glyph to a 16x13 grid even if they'd fit in 8x13 */
					/*
					if (bbxx <= 8) {
						// shift left for 16x13 glyph
						rowout <<= 8;
					}
					*/
					// Honor the glyph's advance
					rowout >>= bbxxoff;
					if (!skip) {
						fprintf(stdout, "%02X", rowout);
						digitsout += 2;
					}
				}

				/* Pad for x13 glyph */
				if (!skip) {
					while (digitsout < (13 * 2)) {
						fprintf(stdout, "00");
						digitsout += 2;
					}
					fprintf(stdout, "\n");
				}
			}
		}
	}
	exit(0);
}

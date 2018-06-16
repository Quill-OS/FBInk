/*
	FBInk: FrameBuffer eInker, a tool to print strings on eInk devices (Kobo/Kindle)
	Copyright (C) 2018 NiLuJe <ninuje@gmail.com>

	Linux framebuffer routines based on: fbtestfnt.c & fbtest6.c, from
	http://raspberrycompote.blogspot.com/2014/04/low-level-graphics-on-raspberry-pi-text.html &
	https://raspberrycompote.blogspot.com/2013/03/low-level-graphics-on-raspberry-pi-part_8.html
	Original works by J-P Rosti (a.k.a -rst- and 'Raspberry Compote'),
	Licensed under the Creative Commons Attribution 3.0 Unported License
	(http://creativecommons.org/licenses/by/3.0/deed.en_US)

	----

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __FBINK_UNSCII_H
#define __FBINK_UNSCII_H

#include <stdint.h>
#include <stdio.h>

#include "fonts/alt.h"
#include "fonts/fantasy.h"
#include "fonts/mcr.h"
#include "fonts/tall.h"
#include "fonts/thin.h"
#include "fonts/unscii.h"

// NOTE: Should technically be pure, but we can get away with const, according to https://lwn.net/Articles/285332/
const char* unscii_get_bitmap(uint32_t) __attribute__((const));
const char* alt_get_bitmap(uint32_t) __attribute__((const));
const char* thin_get_bitmap(uint32_t) __attribute__((const));
const char* fantasy_get_bitmap(uint32_t) __attribute__((const));
const char* mcr_get_bitmap(uint32_t) __attribute__((const));
const char* tall_get_bitmap(uint32_t) __attribute__((const));

#endif

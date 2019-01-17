#ifndef __CUTEF8_DFA_H
#define __CUTEF8_DFA_H

// Because we're pretty much Linux-bound ;).
#ifndef _GNU_SOURCE
#	define _GNU_SOURCE
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 12

size_t u8_strlen2(const char* s) __attribute__((pure));
bool   u8_isvalid2(const char* s) __attribute__((pure));

#endif

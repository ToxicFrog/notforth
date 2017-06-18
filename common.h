#pragma once

#include "config.h"

#ifdef LINUX
  #include <string.h>

  #define STACKSIZE LINUX_STACKSIZE
  #define PROGMEM
  #define PSTR(s) s
  #define pgm_read_byte(x) *(x)
  #define strcpy_P(dst, src) strcpy(dst, src)
  #define strcmp_P(s1, s2) strcmp(s1, s2)
  #define memcpy_P(dst, src, len) memcpy(dst, src, len)
  #define printf_P(...) printf(__VA_ARGS__)
#endif

#ifdef ARDUINO
  #include <avr/pgmspace.h>

  #define STACKSIZE AVR_STACKSIZE
#endif

#define STACKBYTES (STACKSIZE * sizeof(Cell))

#ifndef true
  #define false 0
  #define true (~false)
#endif

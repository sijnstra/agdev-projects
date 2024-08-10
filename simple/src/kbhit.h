/*
 * This code was taken from: 
    [https://github.com/lihexali/linux-kbhit]
*/

#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)

#ifndef _KBHIT_H_
#define _KBHIT_H_

#include "libraries.h"

#define     KEY_ESC     "\033"
#define     KEY_UP      "\033[A"
#define     KEY_DOWN    "\033[B"
#define     KEY_LEFT    "\033[D"
#define     KEY_RIGHT   "\033[C"
#define     KEY_A   "a"
#define     KEY_D   "d"

extern void term_setup(void (*sighandler)(int));
extern void term_restore();
extern bool kbhit();
extern bool keydown(const char* key);

#endif
#endif

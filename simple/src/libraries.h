#ifndef _LIBRARIES_H_
#define _LIBRARIES_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// #include <malloc.h>
#include <math.h>
#include <time.h>


#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
#include <unistd.h>
#include <signal.h>

#elif defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <Windows.h>

#else	//Agon
#include <agon/vdp_vdu.h>
#include <agon/vdp_key.h>
#endif

#endif

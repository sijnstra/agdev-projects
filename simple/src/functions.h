#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "libraries.h"
#include "config.h"

// main map funcitons
extern char** memory_allocate();
extern void create_map();
extern void destruct_map();

// other map funcitons
extern void clear_map(char** map, int16_t numOfFrame);
extern void show_map();
extern void show_info();

// actions funcitons
extern void move(int8_t direction);

// generate funcitons
extern void generate();
extern void generate_landscape(uint32_t min_w, uint16_t min_h, uint32_t max_w, uint16_t max_h, float roughness);
extern void generate_trees(uint32_t min_w, uint32_t max_w);
extern void generate_clouds(uint32_t min_w, uint32_t max_w);
//extern void generate_starts();
//extern void generate_weather();

// supports functions for generate
extern void midpoint_displacement(uint32_t leftX, uint16_t leftY, uint32_t rightX, uint16_t rightY, float roughness);
extern void filling_landscape(uint32_t x, uint16_t start_y);



#endif

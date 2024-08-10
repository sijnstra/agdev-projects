#include "config.h"

sMap worldMap  = {.iMapWidth = SCREEN_WIDTH * NUM_MALLOC_FRAME,
                  .iMapHeight = SCREEN_HEIGHT,
                  .iMapOldWidth = 0,
                  .iMapOldHeight = 0,
                  .iMapFrameCount = NUM_MALLOC_FRAME,
                  .iMapSize = SCREEN_HEIGHT * SCREEN_WIDTH,
                  .iMapOfHeightsSize = 0,
                  .iMapOfCloudsSize = 0,
                  .iMapFullSize = SCREEN_HEIGHT * SCREEN_WIDTH,
                  .map = NULL,
                  .mapOfHeights = NULL,
                  .mapOfClouds = NULL
};


iPoint CameraPosition = {
                  .x = 0,
                  .y = 0
};


char* palette_ground = " .:oO";

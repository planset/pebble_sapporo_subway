#ifndef station_layer_h
#define station_layer_h

#include <pebble.h>

void station_layer_load(Layer *window_layer);
void station_layer_unload(Window *window);
void station_layer_deinit();
void station_layer_update();

#endif

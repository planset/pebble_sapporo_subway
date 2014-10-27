#ifndef watch_h
#define watch_h

#include <pebble.h>

void watch_layer_load(Layer *window_layer);
void watch_layer_unload(Window *window);
void watch_layer_deinit();

#endif

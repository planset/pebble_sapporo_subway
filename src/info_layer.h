#ifndef info_layer_h
#define info_layer_h

#include <pebble.h>

typedef struct {
 
    void (*load)(Layer *window_layer);
    void (*unload)(Window *window);
    void (*deinit)(void);
    Layer *(*get_layer)(void);
 
} IInfoLayer;
 
Layer *info_layer_create();

#endif

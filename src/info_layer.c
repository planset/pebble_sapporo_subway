#include <pebble.h>

#include "common.h"
#include "info_layer.h"

Layer *info_layer_create(){
    return layer_create(
            GRect(INFO_LAYER_LEFT, INFO_LAYER_TOP, 
                INFO_LAYER_WIDTH, INFO_LAYER_HEIGHT));
}

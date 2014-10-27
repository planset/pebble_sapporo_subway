#ifndef info_layer_state_h
#define info_layer_state_h

#include <pebble.h>

#include "info_layer.h"

IInfoLayer *StateLayer_Create();

//void create_battery_option();
//void battery_layer_update_callback(Layer *, GContext *);
//void battery_state_handler(BatteryChargeState);

#define ICON_WIDTH = 24
#define ICON_HEIGHT = 12

#endif

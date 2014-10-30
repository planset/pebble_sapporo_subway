#ifndef common_h
#define common_h

#include <pebble.h>

#define WATCH_LAYER_LEFT 0
#define WATCH_LAYER_TOP 0
#define WATCH_LAYER_WIDTH 144
#define WATCH_LAYER_HEIGHT 78

#define INFO_LAYER_LEFT 0
#define INFO_LAYER_TOP 79
#define INFO_LAYER_WIDTH 144
#define INFO_LAYER_HEIGHT 90

enum CommunicationKey {
    COMMAND_KEY = 0,            // TUPLE_INTEGER
                                // 1: 
                                // 2: Get nearest station info

    STATION_INFO_NAME = 20,          // TUPLE_STRING
    STATION_INFO_DIR_1 = 21,          // TUPLE_STRING
    STATION_INFO_DIR_2 = 22,          // TUPLE_STRING
    STATION_INFO_DIR_1_DEPARTURE = 23,          // TUPLE_STRING
    STATION_INFO_DIR_2_DEPARTURE = 24,          // TUPLE_STRING

    __COMMUNICATION_KEY_END__ = 99    //dummy
};

enum CommandKey {
    START_CHECK_STATION = 20,
    STOP_CHECK_STATION = 21,
    __COMMAND_KEY_END__ = 99    //dummy
};


#endif

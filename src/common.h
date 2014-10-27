#ifndef common_h
#define common_h

#include <pebble.h>

#define RECONNECT_KEY 0
#define REQUEST_CALENDAR_KEY 1
#define CLOCK_STYLE_KEY 2
#define CALENDAR_RESPONSE_KEY 3
#define REQUEST_BATTERY_KEY 8
#define BATTERY_RESPONSE_KEY 9

#define CLOCK_STYLE_12H 1
#define CLOCK_STYLE_24H 2

#define WINDOW_LEFT 0
#define WINDOW_TOP 0

#define WINDOW_WIDTH PEBBLE_SCREEN_WIDTH
#define WINDOW_HEIGHT PEBBLE_SCREEN_HEIGHT

#define WATCH_LAYER_LEFT 0
#define WATCH_LAYER_TOP 0
#define WATCH_LAYER_WIDTH 144
#define WATCH_LAYER_HEIGHT 78

#define INFO_LAYER_LEFT 0
#define INFO_LAYER_TOP 79
#define INFO_LAYER_WIDTH 144
#define INFO_LAYER_HEIGHT 90

typedef struct {
    uint8_t index;
    char title[21];
    bool has_location;
    char location[21];
    bool all_day;
    char start_date[18];
    int32_t alarms[2];
} Event;

enum CommunicationKey {
    COMMAND_KEY = 0,            // TUPLE_INTEGER
                                // 1: 
                                // 2: Get nearest station info
    MESSAGE = 10,               // TUPLE_STRING

    STATION_INFO_NAME = 20,          // TUPLE_STRING
    STATION_INFO_DIR_1 = 21,          // TUPLE_STRING
    STATION_INFO_DIR_2 = 22,          // TUPLE_STRING
    STATION_INFO_DIR_1_DEPARTURE = 23,          // TUPLE_STRING
    STATION_INFO_DIR_2_DEPARTURE = 24,          // TUPLE_STRING

    __COMMUNICATION_KEY_END__ = 99    //dummy
};

enum CommandKey {
    START_CHECK_MESSAGE = 10,
    STOP_CHECK_MESSAGE = 11,
    START_CHECK_STATION = 20,
    STOP_CHECK_STATION = 21,
    __COMMAND_KEY_END__ = 99    //dummy
};


#endif

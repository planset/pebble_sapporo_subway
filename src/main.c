#include "pebble.h"
#include "pebble_fonts.h"

#include "common.h"
#include "util.h"
#include "watch.h"

#include "station_layer.h"

/*
 * main window
 */
static Window *window;


/*
 * 時計をtap(えいっ！と回す（ふる））のハンドラ
 */
static void accel_tap_handler(AccelAxisType axis, int32_t direction ) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "accel tap");

    station_layer_update();
}


/*
 * load: called when the window is pushed to the screen when it's not loaded. This is a good moment to do the layout of the window.
 */
static void window_load(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "load");

    Layer *window_layer = window_get_root_layer(window);

    //
    // 時計表示部：0, 0, 144, 78
    // 
    watch_layer_load(window_layer);


    // 
    // 情報表示部：0, 79, 144, 90
    // 
    station_layer_load(window_layer);


    //
    // 情報更新用のふるふる処理
    //
    accel_tap_service_subscribe(accel_tap_handler);

}

/*
 * window unload
 */
static void window_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "unload");

    watch_layer_unload(window);
    station_layer_unload(window);
}


/*
 * application initialize
 */
static void init(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "init");

    /*
     * Create Window
     */
    window = window_create();
    window_set_background_color(window, GColorBlack);
    window_set_fullscreen(window, true);
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
            });

    const bool animated = true;
    window_stack_push(window, animated);
}

/*
 * deinit
 */
static void deinit(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit");

    watch_layer_deinit();
    station_layer_deinit();
    window_destroy(window);
}

/*
 * main
 */
int main(void) {
    init();
    app_event_loop();
    deinit();
}

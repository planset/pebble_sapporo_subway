#include "pebble.h"
#include "pebble_fonts.h"

#include "common.h"
#include "util.h"
#include "watch.h"

#include "info_layer.h"
#include "info_layer_state.h"
//#include "info_layer_message.h"
#include "info_layer_station.h"

static Window *window;

// 情報表示レイヤ
static IInfoLayer *info_layer = NULL;

// 情報表示レイヤの生成処理を追加する
static IInfoLayer *(*info_layers_create[])() = {
    StateLayer_Create,
    //MessageLayer_Create,
    StationLayer_Create
};

// 現在表示している情報表示レイヤのインデックス
static int info_layer_index = 0;

// 登録した情報表示レイヤの数
static int info_layers_len;

// 使用できる情報表示レイヤの配列
// info_layers_createの長さ分info_layersに領域を確保して使う
static IInfoLayer * (*info_layers);


/*
 * 情報表示レイヤから現在表示しているレイヤを削除する
 */
static void remove_info_layer() {
    if (info_layer != NULL) {
        layer_remove_from_parent(info_layer->get_layer());
        info_layer->unload(window);
        info_layer->deinit();
        info_layer = NULL;
    }
}

/*
 * 情報表示レイヤにレイヤを追加する
 */
static void add_info_layer() {
    info_layer = (IInfoLayer*)info_layers[info_layer_index];

    Layer *window_layer = window_get_root_layer(window);
    info_layer->load(window_layer);

    layer_add_child(window_layer, info_layer->get_layer());
}

/*
 * 次の情報表示レイヤに切り替える
 */
static void next_info_layer() {
    remove_info_layer();

    info_layer_index++;
    if (info_layer_index == info_layers_len) {
        info_layer_index = 0;
    }

    add_info_layer();
}

/*
 * 加速度計のハンドラ
 */
static void accel_tap_handler(AccelAxisType axis, int32_t direction ) {

    APP_LOG(APP_LOG_LEVEL_DEBUG, "accel tap");

    next_info_layer();
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

    // 情報表示用レイヤを生成する。
    // TODO: 表示する時に生成する方がよいかも
    info_layers_len = sizeof(info_layers_create) / sizeof(info_layers_create[0]);
    info_layers = (IInfoLayer **)malloc(sizeof(IInfoLayer *) * info_layers_len);
    for (int i=0; i<info_layers_len; i++) {
        info_layers[i] = info_layers_create[i]();
    }
    info_layer_index = info_layers_len - 1;

    next_info_layer();

    //
    // 情報表示部切り替え用のふるふる処理
    //
    accel_tap_service_subscribe(accel_tap_handler);

}



/* 
 * unload: called when the window is deinited, but could be used in the future to free resources bound to windows that are not on screen.
 * */
static void window_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "unload");

    watch_layer_unload(window);
    remove_info_layer();
}


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

static void deinit(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit");

    free(info_layers);
    watch_layer_deinit();
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}

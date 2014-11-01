#include <pebble.h>

#include "common.h"
#include "message.h"
#include "info_layer_station.h"

/*
 * 駅情報を載せるレイヤ
 */
static Layer *info_layer;

/*
 * 近くの駅名を表示する
 */
static TextLayer *text_station_name_layer;

/*
 * 「栄町」を表示する
 */
static TextLayer *text_station_direction_1_layer;

/*
 * 栄町行きの次の発車時刻を表示する
 */
static TextLayer *text_station_direction_2_layer;

/*
 * 「福住」を表示する
 */
static TextLayer *text_station_direction_1_departure_layer;

/*
 * 福住行きの次の発車時刻を表示する
 */
static TextLayer *text_station_direction_2_departure_layer;

/*
 * JS側から駅情報を受け取った時の処理
 */
static void received_station(DictionaryIterator *received, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "received_station");

    Tuple *station_name = dict_find(received, STATION_INFO_NAME);
    if (station_name) {
        text_layer_set_text(text_station_name_layer, 
                station_name->value->cstring);
    }
    Tuple *station_dir_1 = dict_find(received, STATION_INFO_DIR_1);
    if (station_dir_1) {
        text_layer_set_text(text_station_direction_1_layer, 
                station_dir_1->value->cstring);
    }
    Tuple *station_dir_2 = dict_find(received, STATION_INFO_DIR_2);
    if (station_dir_2) {
        text_layer_set_text(text_station_direction_2_layer, 
                station_dir_2->value->cstring);
    }
    Tuple *station_dir_1_departure = dict_find(received, STATION_INFO_DIR_1_DEPARTURE);
    if (station_dir_1_departure) {
        text_layer_set_text(text_station_direction_1_departure_layer, 
                station_dir_1_departure->value->cstring);
    }
    Tuple *station_dir_2_departure = dict_find(received, STATION_INFO_DIR_2_DEPARTURE);
    if (station_dir_2_departure) {
        text_layer_set_text(text_station_direction_2_departure_layer, 
                station_dir_2_departure->value->cstring);
    }

}

/*
 * ヘルパ関数：GRectからテキストレイヤを作成して返す
 */
static TextLayer *create_my_text_layer(GRect grect) {
    TextLayer *tl = text_layer_create(grect);
    //text_layer_set_overflow_mode(tl, GTextOverflowModeWordWrap);
    text_layer_set_text_color(tl, GColorWhite);
    text_layer_set_background_color(tl, GColorClear);
    return tl;
}

/*
 * windowがロードされた際に呼び出されるレイヤの初期化処理
 */
static void info_layer_station_load(Layer *window_layer) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_station_load");

    //
    static bool is_registered_callback = false;

    info_layer = info_layer_create();

    // width: 144px
    // height: 90px
    // margin: 0 2px 0 2px;
    text_station_name_layer = create_my_text_layer(GRect(0+2, 0+5, 144-2, 30));
    text_layer_set_font(text_station_name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(info_layer, text_layer_get_layer(text_station_name_layer));

    text_station_direction_1_layer = create_my_text_layer(GRect(0+2, 30+2, 90, 30));
    //text_layer_set_overflow_mode(text_station_direction_1_layer, GTextOverflowModeWordWrap);
    text_layer_set_font(text_station_direction_1_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(info_layer, text_layer_get_layer(text_station_direction_1_layer));

    text_station_direction_1_departure_layer = create_my_text_layer(
            GRect(0+2+90-5, 30, 50, 30));
    text_layer_set_font(text_station_direction_1_departure_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_STATION_20)));
    layer_add_child(info_layer, text_layer_get_layer(text_station_direction_1_departure_layer));

    text_station_direction_2_layer = create_my_text_layer(GRect(0+2, 60+2, 90, 30));
    text_layer_set_font(text_station_direction_2_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    layer_add_child(info_layer, text_layer_get_layer(text_station_direction_2_layer));

    text_station_direction_2_departure_layer = create_my_text_layer(
            GRect(0+2+90-5, 60, 50, 30));
    text_layer_set_font(text_station_direction_2_departure_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_STATION_20)));
    layer_add_child(info_layer, text_layer_get_layer(text_station_direction_2_departure_layer));

    // メッセージの受信設定
    if (is_registered_callback == false) {
        const int inbound_size = 124;
        const int outbound_size = 124;
        app_message_open(inbound_size, outbound_size);
        app_message_register_inbox_received(received_station);
        is_registered_callback = true;
    }

    // メッセージ受信の開始
    send_cmd(START_CHECK_STATION, text_station_direction_1_departure_layer);
}

/* windows unload  */
static void info_layer_station_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_station_unload");

    send_cmd(STOP_CHECK_STATION, text_station_direction_1_departure_layer);
    
    //app_station_deregister_callbacks();

    text_layer_destroy(text_station_name_layer);
    text_layer_destroy(text_station_direction_1_layer);
    text_layer_destroy(text_station_direction_2_layer);
    text_layer_destroy(text_station_direction_1_departure_layer);
    text_layer_destroy(text_station_direction_2_departure_layer);
    layer_destroy(info_layer);
}

/*
 * deinit
 */
static void info_layer_station_deinit() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_station_deinit");
}

/*
 * return info_layer
 */
static Layer *info_layer_station_get_layer() {
    return info_layer;
}

/*
 * info_layerの構造体
 */
static IInfoLayer info_layer_station = {
    info_layer_station_load,      /* load */
    info_layer_station_unload,    /* unload */
    info_layer_station_deinit,    /* deinit */
    info_layer_station_get_layer  /* get_layer */
};

/*
 * return info_layer_station
 */
IInfoLayer *StationLayer_Create() {
    return &info_layer_station;
}


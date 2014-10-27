#include <pebble.h>

#include "common.h"
#include "message.h"
#include "info_layer_message.h"

// メインのレイヤー　これにのせたい情報をのせる。
static Layer *info_layer;

// メッセージ表示用レイヤ
static TextLayer *text_message_layer;

//
static bool is_registered_callback = false;

// メッセージのdictionaryの中身を表示
void debug_message(DictionaryIterator *received) {
    Tuple *data = dict_read_first(received);
    while (data != NULL) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "key=%ld", data->key);
        switch (data->type) {
            case TUPLE_CSTRING:
                APP_LOG(APP_LOG_LEVEL_DEBUG, "cstring: %s", data->value->cstring);
                break;
            case TUPLE_BYTE_ARRAY:
                APP_LOG(APP_LOG_LEVEL_DEBUG, "BYTE_ARRAY");
                break;
            case TUPLE_UINT:
                APP_LOG(APP_LOG_LEVEL_DEBUG, "%ud", data->value->uint8);
                break;
            case TUPLE_INT:
                APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", data->value->int8);
                break;
        }
        data = dict_read_next(received);
    }
}
// 
static void received_message(DictionaryIterator *received, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "received_message");

    //debug_message(received);

    Tuple *short_message = dict_find(received, MESSAGE);
    if (short_message) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "received_message");
        text_layer_set_text(text_message_layer, short_message->value->cstring);
    }
}

/* windows load  */
static void info_layer_message_load(Layer *window_layer) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_message_load");

    info_layer = info_layer_create();

    // 
    text_message_layer = text_layer_create(GRect(0, 0, 144, 90));
    text_layer_set_overflow_mode(text_message_layer, GTextOverflowModeWordWrap);
    text_layer_set_text_color(text_message_layer, GColorWhite);
    text_layer_set_background_color(text_message_layer, GColorClear);
    text_layer_set_font(text_message_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_MESSAGE_20)));
    text_layer_set_text(text_message_layer, "messenger");
    layer_add_child(info_layer, text_layer_get_layer(text_message_layer));

    // メッセージの受信設定
    if (is_registered_callback == false) {
        const int inbound_size = 124;
        const int outbound_size = 124;
        app_message_open(inbound_size, outbound_size);
        app_message_register_inbox_received(received_message);
        is_registered_callback = true;
    }

    // メッセージ受信の開始
    send_cmd(START_CHECK_MESSAGE, text_message_layer);
}

/* windows unload  */
static void info_layer_message_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_message_unload");

    send_cmd(STOP_CHECK_MESSAGE, text_message_layer);
    
    //app_message_deregister_callbacks();

    text_layer_destroy(text_message_layer);
    layer_destroy(info_layer);
}

/* app deinit */
static void info_layer_message_deinit() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_message_deinit");
}

static Layer *info_layer_message_get_layer() {
    return info_layer;
}

static IInfoLayer info_layer_message = {
    info_layer_message_load,      /* load */
    info_layer_message_unload,    /* unload */
    info_layer_message_deinit,    /* deinit */
    info_layer_message_get_layer  /* get_layer */
};

IInfoLayer *MessageLayer_Create() {
    return &info_layer_message;
}


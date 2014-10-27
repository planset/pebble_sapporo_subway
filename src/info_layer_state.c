#include <pebble.h>

#include "common.h"
#include "info_layer_state.h"

// メインのレイヤー　これにのせたい情報をのせる。
static Layer *info_layer;

// バッテリーの充電状態を文字で表示するレイヤー
static TextLayer *text_battery_layer;

// バッテリー用のicon画像
static GBitmap *icon_battery;

// バッテリーアイコン描画用レイヤー
static Layer *icon_battery_layer;


/*
 * バッテリーの状態が変更になった場合のcallback
 * テキストを変更する
 */
static void battery_state_handler(BatteryChargeState charge) {

  char* battery_text = "* xxx%  ";
  size_t tlen = strlen(battery_text);
  char state = ' ';
  if (charge.is_charging) {
    state = '*';
  } else if (charge.is_plugged) {
    state = '=';
  }

  snprintf(battery_text, tlen, 
      "%c %3d%% ",
       state, charge.charge_percent);

  text_layer_set_text(text_battery_layer, battery_text);

}


/*
 * バッテリーの状態をresources/images/battery_icon.pngを使って表示する。
 * Image is width 24 * height 12 pixels.
 */
static void battery_layer_update_callback(Layer *layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "battery_layer_update_callback");

  graphics_context_set_compositing_mode(ctx, GCompOpAssignInverted);
  graphics_draw_bitmap_in_rect(ctx, icon_battery, GRect(35, 0, 24, 12));

  BatteryChargeState bts = battery_state_service_peek();

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(38, 3, (uint8_t)((bts.charge_percent / 100.0) * 16.0), 6), 0, GCornerNone);
}


/* windows load  */
static void info_layer_state_load(Layer *window_layer) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_state_load");

    info_layer = info_layer_create();

    // バッテリーアイコンが右から幅66/2使っている。
    text_battery_layer = text_layer_create(GRect(0, 2, 144 - 33, 18));
    text_layer_set_text_color(text_battery_layer, GColorWhite);
    text_layer_set_text_alignment(text_battery_layer, GTextAlignmentRight);
    text_layer_set_background_color(text_battery_layer, GColorClear);
    text_layer_set_font(text_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    layer_add_child(info_layer, text_layer_get_layer(text_battery_layer));

    /*
     * Create BatteryLayer
     */
    icon_battery = gbitmap_create_with_resource(RESOURCE_ID_BATTERY_ICON);

    GRect frame;
    frame.origin.x = layer_get_bounds(info_layer).size.w - 66;
    frame.origin.y = 6;
    frame.size.w = 59;
    frame.size.h = 12;

    icon_battery_layer = layer_create(frame);
    layer_set_update_proc(icon_battery_layer, battery_layer_update_callback);
    layer_add_child(info_layer, icon_battery_layer);


    // 初回描画
    BatteryChargeState bts = battery_state_service_peek();
    battery_state_handler(bts);
    battery_state_service_subscribe(&battery_state_handler);
}

/* windows unload  */
static void info_layer_state_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_state_unload");

    battery_state_service_unsubscribe();

    text_layer_destroy(text_battery_layer);
    gbitmap_destroy(icon_battery);
    layer_destroy(icon_battery_layer);
    layer_destroy(info_layer);
}

/* app deinit */
static void info_layer_state_deinit() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_state_deinit");
}

static Layer *info_layer_state_get_layer() {
    return info_layer;
}

static IInfoLayer info_layer_state = {
    info_layer_state_load,      /* load */
    info_layer_state_unload,    /* unload */
    info_layer_state_deinit,    /* deinit */
    info_layer_state_get_layer  /* get_layer */
};

IInfoLayer *StateLayer_Create() {
    return &info_layer_state;
}


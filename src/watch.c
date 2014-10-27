#include <pebble.h>
#include "watch.h"

static TextLayer *text_date_layer;
static TextLayer *text_time_layer;


static struct WatchUi {
    TextLayer *text_date_layer;
    TextLayer *text_time_layer;
    char time_text[5];
    char date_text[9];
} ui;

TextLayer* _create_time_layer(int left, int top) {
    TextLayer *layer;

    layer = text_layer_create(GRect(left, top, 144-left, 52));
    text_layer_set_text_color(layer, GColorWhite);
    text_layer_set_background_color(layer, GColorClear);
    text_layer_set_font(layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_48)));

    return layer;
}

TextLayer* _create_date_layer(int left, int top) {
    TextLayer *layer;

    layer = text_layer_create(GRect(left, top, 144-left, 28));
    text_layer_set_text_color(layer, GColorWhite);
    text_layer_set_background_color(layer, GColorClear);
    text_layer_set_font(layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DATE_24)));

    return layer;
}

void _update_time(TextLayer *layer, struct tm *tick_time) {
  static char time_text[] = "00:00";
  char *time_format;

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(layer, time_text);
}

void _update_date(TextLayer *layer, struct tm *tick_time) {
    static char date_text[] = "00/00 Xxxxxxx";

    strftime(date_text, sizeof(date_text), "%m/%e %a", tick_time);
    text_layer_set_text(layer, date_text);
}

static void _tick_timer_handler(struct tm *tick_time, TimeUnits units_changed) {
    _update_time(text_time_layer, tick_time);
    _update_date(text_date_layer, tick_time);
}

/* 初回の描画 */
void _initialize_layer_text(){
    time_t t;
    time(&t);
    struct tm *tick_time = localtime(&t);
    _update_date(text_date_layer, tick_time);
    _update_time(text_time_layer, tick_time);
}


/* windows load  */
void watch_layer_load(Layer *window_layer) {

    // 時刻表示
    text_time_layer = _create_time_layer(4, 0);
    layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

    // 日付表示 78-24=54
    text_date_layer = _create_date_layer(8, 54);
    layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

    // 時刻、日付更新Callback
    tick_timer_service_subscribe(MINUTE_UNIT, &_tick_timer_handler);

    _initialize_layer_text();
}

/* windows unload  */
void watch_layer_unload(Window *window) {
    text_layer_destroy(text_time_layer);
    text_layer_destroy(text_date_layer);
}

/* app deinit */
void watch_layer_deinit() {
    tick_timer_service_unsubscribe();
}


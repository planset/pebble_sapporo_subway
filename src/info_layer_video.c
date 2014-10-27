#include <pebble.h>

#include "common.h"
#include "info_layer_video.h"
#include "netimage.h"

// メインのレイヤー　これにのせたい情報をのせる。
static Layer *info_layer;

static BitmapLayer *bitmap_layer;
static GBitmap *current_image;


void display_image(GBitmap *image) {
  bitmap_layer_set_bitmap(bitmap_layer, image);
  // Free the memory used by the previous image
  if (current_image) {
    free(current_image);
  }
  // Keep a pointer to this image data so we can free it later.
  current_image = image;
}

void tap_handler(AccelAxisType accel, int32_t direction) {
  if (++image > sizeof(images)) image = 0;
  netimage_request(images[image]);

  // show that we are loading...
  display_image(NULL);
}


/* windows load  */
static void info_layer_video_load(Layer *window_layer) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_video_load");

    info_layer = info_layer_create();

    GRect bounds = layer_get_bounds(window_layer);

    text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
    text_layer_set_text(text_layer, "Loading...");
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    layer_add_child(info_layer, text_layer_get_layer(text_layer));

    bitmap_layer = bitmap_layer_create(bounds);
    layer_add_child(info_layer, bitmap_layer_get_layer(bitmap_layer));
    current_image = NULL;

    netimage_initialize(display_image);
}

/* windows unload  */
static void info_layer_video_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_video_unload");

    text_layer_destroy(text_layer);
    bitmap_layer_destroy(bitmap_layer);
    if (current_image) {
        free(current_image);
    }
    layer_destroy(info_layer);

    netimage_deinitialize(); // call this to avoid 20B memory leak
}

/* app deinit */
static void info_layer_video_deinit() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "info_layer_video_deinit");
}

static Layer *info_layer_video_get_layer() {
    return info_layer;
}

static IInfoLayer info_layer_video = {
    info_layer_video_load,      /* load */
    info_layer_video_unload,    /* unload */
    info_layer_video_deinit,    /* deinit */
    info_layer_video_get_layer  /* get_layer */
};

IInfoLayer *StateLayer_Create() {
    return &info_layer_video;
}


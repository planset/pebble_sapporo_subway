#ifndef info_layer_h
#define info_layer_h

#include <pebble.h>

typedef struct {
 
    /*
     * loadの中で表示したいレイヤを作りwindow_layerに追加する
     */
    void (*load)(Layer *window_layer);

    /*
     * unload処理
     */
    void (*unload)(Window *window);

    /*
     * deinit処理
     */
    void (*deinit)(void);

    /*
     * 作成したレイヤを返す
     */
    Layer *(*get_layer)(void);
 
} IInfoLayer;
 
/*
 * 
 */
Layer *info_layer_create();

#endif

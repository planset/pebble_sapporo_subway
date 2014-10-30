#include <pebble.h>

#include "common.h"
#include "message.h"

/*
 * メッセージ送信
 */
void send_cmd(int command_id, TextLayer *error_text_layer) {
    Tuplet value = TupletInteger(COMMAND_KEY, command_id);

    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    if (iter == NULL) {
        if (error_text_layer != NULL) {
            text_layer_set_text(error_text_layer, "Error! : Send command");
        }
        return;
    }

    dict_write_tuplet(iter, &value);
    dict_write_end(iter);

    app_message_outbox_send();
}


======
README
======

メモ


イベントの順序
===============

起動時
--------
::

    [INFO    ] D ebble-planset.c:112 init
    [INFO    ] D ebble-planset.c:73 load
    [INFO    ] D o_layer_state.c:103 info_layer_state_load
    [INFO    ] D ebble-planset.c:128 done initializing
    [INFO    ] JS: starting app: ED782D0C-5AD0-42F3-9B53-AE6DF451FC89 pebble-planset
    [INFO    ] D o_layer_state.c:88 battery_layer_update_callback
    [INFO    ] app is ready: 1
    [INFO    ] JS: pebble-planset: Hello world! - Sent from your javascript application.
    [INFO    ] D o_layer_state.c:88 battery_layer_update_callback


他のWatchFaceに切り替えた時（unload）
--------------------------------------
::

    [INFO    ] D ebble-planset.c:104 unload
    [INFO    ] D o_layer_state.c:154 info_layer_state_unload
    [INFO    ] D o_layer_state.c:166 info_layer_state_deinit
    [INFO    ] D ebble-planset.c:132 deinit
    [INFO    ] I app_manager.c:138 Heap Usage for <pebble-planset>: Total Size <18880B> Used <2548B> Still allocated <32B>
    [INFO    ] JS: stopping app: ED782D0C-5AD0-42F3-9B53-AE6DF451FC89 pebble-planset


このWatchFaceに切り替えた時（load）
--------------------------------------
::

    [INFO    ] JS: starting app: 84678888-13D8-41DC-BA56-47F88724DEA5 Studio Clock
    [INFO    ] app is ready: 1
    [INFO    ] JS: Studio Clock: Showing Weather for City: Sapporo-shi, JP
    [INFO    ] I app_manager.c:138 Heap Usage for <Studio Clock>: Total Size <14948B> Used <3136B> Still allocated <0B>
    [INFO    ] D ebble-planset.c:112 init
    [INFO    ] D ebble-planset.c:73 load
    [INFO    ] D o_layer_state.c:103 info_layer_state_load
    [INFO    ] D ebble-planset.c:128 done initializing







#include "zw_game_bang.h"

#include "zw_game_zombie.h"
#include "zw_game_bullet.h"

zw_game_bang_t bang[NUM_BANG];

#define ZW_GAME_BANG_SETUP() \
do { \
    for (uint8_t i = 0; i < NUM_BANG; i++) { \
        bang[i].x = 0; \
        bang[i].y = 0; \
        bang[i].visible = BLACK; \
        bang[i].action_image = 1; \
    } \
} while (0);

#define ZW_GAME_BANG_UPDATE() \
do { \
    for (uint8_t i = 0; i < NUM_BANG; i++) { \
        if (bang[i].visible == WHITE) { \
            bang[i].action_image++; \
        } \
        if (bang[i].action_image == 4) { \
            bang[i].action_image = 1; \
            bang[i].visible = BLACK; \
            zombie[i].visible = WHITE; \
        } \
    } \
} while(0);

#define ZW_GAME_BANG_RESET() \
do { \
    for (uint8_t i = 0; i < NUM_BANG; i++) { \
        bang[i].visible = BLACK; \
        bang[i].action_image = 1; \
    } \
} while (0);

void zw_game_bang_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_BANG_SETUP: {
        APP_DBG_SIG("ZW_GAME_BANG_SETUP\n");
        ZW_GAME_BANG_SETUP();
    }
        break;

    case ZW_GAME_BANG_UPDATE: {
        APP_DBG_SIG("ZW_GAME_BANG_UPDATE\n");
        ZW_GAME_BANG_UPDATE();
    }
        break;

    case ZW_GAME_BANG_RESET: {
        APP_DBG_SIG("ZW_GAME_BANG_RESET\n");
        ZW_GAME_BANG_RESET();
    }
        break;

    default:
        break;
    }
}
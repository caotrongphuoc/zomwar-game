#include "ar_game_zomwar.h"

ar_game_zomwar_t zomwar;
static uint32_t zomwar_y = AXIS_Y_ZOMWAR;
volatile zomwar_move_dir_t zomwar_moving = ZOMWAR_MOVE_NONE;

#define AR_GAME_ZOMWAR_SETUP() \
do { \
    zomwar.x = AXIS_X_ZOMWAR; \
    zomwar.y = AXIS_Y_ZOMWAR; \
    zomwar.visible = WHITE; \
    zomwar.action_image = 1; \
    zomwar_moving = ZOMWAR_MOVE_NONE; \
    zomwar_y = AXIS_Y_ZOMWAR; \
} while (0);

// UPDATE giờ check flag mỗi tick → di chuyển liên tục
#define AR_GAME_ZOMWAR_UPDATE() \
do { \
    if (zomwar_moving == ZOMWAR_MOVE_UP) { \
        zomwar_y -= STEP_ZOMWAR_AXIS_Y; \
        if (zomwar_y == 0) { zomwar_y = 10; } \
    } \
    else if (zomwar_moving == ZOMWAR_MOVE_DOWN) { \
        zomwar_y += STEP_ZOMWAR_AXIS_Y; \
        if (zomwar_y > 50) { zomwar_y = 50; } \
    } \
    zomwar.y = zomwar_y; \
    zomwar.action_image = 1; \
} while(0);

#define AR_GAME_ZOMWAR_RESET() \
do { \
    zomwar.x = AXIS_X_ZOMWAR; \
    zomwar.y = AXIS_Y_ZOMWAR; \
    zomwar.visible = BLACK; \
    zomwar_y = AXIS_Y_ZOMWAR; \
    zomwar_moving = ZOMWAR_MOVE_NONE; \
} while(0);

void ar_game_zomwar_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_ZOMWAR_SETUP: {
        APP_DBG_SIG("AR_GAME_ZOMWAR_SETUP\n");
        AR_GAME_ZOMWAR_SETUP();
    }
        break;
    case AR_GAME_ZOMWAR_UPDATE: {
        APP_DBG_SIG("AR_GAME_ZOMWAR_UPDATE\n");
        AR_GAME_ZOMWAR_UPDATE();
    }
        break;
    default:
        break;
    }
}
#include "zw_game_zomwar.h"

typedef enum {
    ZOMWAR_MOVE_NONE = 0,
    ZOMWAR_MOVE_UP,
    ZOMWAR_MOVE_DOWN,
} zomwar_move_dir_t;

zw_game_zomwar_t zomwar;
static uint32_t zomwar_y = AXIS_Y_ZOMWAR;
static zomwar_move_dir_t zomwar_moving = ZOMWAR_MOVE_NONE;

#define ZW_GAME_ZOMWAR_SETUP() \
do { \
    zomwar.x = AXIS_X_ZOMWAR; \
    zomwar.y = AXIS_Y_ZOMWAR; \
    zomwar.visible = WHITE; \
    zomwar.action_image = 1; \
    zomwar_moving = ZOMWAR_MOVE_NONE; \
    zomwar_y = AXIS_Y_ZOMWAR; \
} while (0);

// UPDATE giờ check flag mỗi tick → di chuyển liên tục
#define ZW_GAME_ZOMWAR_UPDATE() \
do { \
    if (zomwar_moving == ZOMWAR_MOVE_UP) { \
        zomwar_y -= STEP_ZOMWAR_AXIS_Y; \
        if (zomwar_y == 2) { zomwar_y = 12; } \
    } \
    else if (zomwar_moving == ZOMWAR_MOVE_DOWN) { \
        zomwar_y += STEP_ZOMWAR_AXIS_Y; \
        if (zomwar_y > 52) { zomwar_y = 52; } \
    } \
    zomwar.y = zomwar_y; \
    zomwar.action_image = 1; \
} while(0);

#define ZW_GAME_ZOMWAR_RESET() \
do { \
    zomwar.x = AXIS_X_ZOMWAR; \
    zomwar.y = AXIS_Y_ZOMWAR; \
    zomwar.visible = BLACK; \
    zomwar_y = AXIS_Y_ZOMWAR; \
    zomwar_moving = ZOMWAR_MOVE_NONE; \
} while(0);

void zw_game_zomwar_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_ZOMWAR_SETUP: {
        APP_DBG_SIG("ZW_GAME_ZOMWAR_SETUP\n");
        ZW_GAME_ZOMWAR_SETUP();
    }
        break;
    case ZW_GAME_ZOMWAR_UPDATE: {
        APP_DBG_SIG("ZW_GAME_ZOMWAR_UPDATE\n");
        ZW_GAME_ZOMWAR_UPDATE();
    }
        break;
    case ZW_GAME_ZOMWAR_UP: {
        APP_DBG_SIG("ZW_GAME_ZOMWAR_UP\n");
        zomwar_moving = ZOMWAR_MOVE_UP;
    }
        break;
    case ZW_GAME_ZOMWAR_DOWN: {
        APP_DBG_SIG("ZW_GAME_ZOMWAR_DOWN\n");
        zomwar_moving = ZOMWAR_MOVE_DOWN;
    }
        break;
    case ZW_GAME_ZOMWAR_STOP: {
        APP_DBG_SIG("ZW_GAME_ZOMWAR_STOP\n");
        zomwar_moving = ZOMWAR_MOVE_NONE;
    }
        break;
    default:
        break;
    }
}
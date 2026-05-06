#include "zw_game_peashooter.h"

typedef enum {
    PEASHOOTER_MOVE_NONE = 0,
    PEASHOOTER_MOVE_UP,
    PEASHOOTER_MOVE_DOWN,
} peashooter_move_dir_t;

zw_game_peashooter_t peashooter;
static uint32_t peashooter_y = AXIS_Y_PEASHOOTER;
static peashooter_move_dir_t peashooter_moving = PEASHOOTER_MOVE_NONE;

#define ZW_GAME_PEASHOOTER_SETUP() \
do { \
    peashooter.x = AXIS_X_PEASHOOTER; \
    peashooter.y = AXIS_Y_PEASHOOTER; \
    peashooter.visible = WHITE; \
    peashooter.action_image = 1; \
    peashooter_moving = PEASHOOTER_MOVE_NONE; \
    peashooter_y = AXIS_Y_PEASHOOTER; \
} while (0);

// UPDATE giờ check flag mỗi tick → di chuyển liên tục
#define ZW_GAME_PEASHOOTER_UPDATE() \
do { \
    if (peashooter_moving == PEASHOOTER_MOVE_UP) { \
        peashooter_y -= STEP_PEASHOOTER_AXIS_Y; \
        if (peashooter_y == 2) { peashooter_y = 12; } \
    } \
    else if (peashooter_moving == PEASHOOTER_MOVE_DOWN) { \
        peashooter_y += STEP_PEASHOOTER_AXIS_Y; \
        if (peashooter_y > 52) { peashooter_y = 52; } \
    } \
    peashooter.y = peashooter_y; \
    peashooter.action_image = 1; \
} while(0);

#define ZW_GAME_PEASHOOTER_RESET() \
do { \
    peashooter.x = AXIS_X_PEASHOOTER; \
    peashooter.y = AXIS_Y_PEASHOOTER; \
    peashooter.visible = BLACK; \
    peashooter_y = AXIS_Y_PEASHOOTER; \
    peashooter_moving = PEASHOOTER_MOVE_NONE; \
} while(0);

void zw_game_peashooter_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_PEASHOOTER_SETUP: {
        APP_DBG_SIG("ZW_GAME_PEASHOOTER_SETUP\n");
        ZW_GAME_PEASHOOTER_SETUP();
    }
        break;
    case ZW_GAME_PEASHOOTER_UPDATE: {
        APP_DBG_SIG("ZW_GAME_PEASHOOTER_UPDATE\n");
        ZW_GAME_PEASHOOTER_UPDATE();
    }
        break;
    case ZW_GAME_PEASHOOTER_UP: {
        APP_DBG_SIG("ZW_GAME_PEASHOOTER_UP\n");
        peashooter_moving = PEASHOOTER_MOVE_UP;
    }
        break;
    case ZW_GAME_PEASHOOTER_DOWN: {
        APP_DBG_SIG("ZW_GAME_PEASHOOTER_DOWN\n");
        peashooter_moving = PEASHOOTER_MOVE_DOWN;
    }
        break;
    case ZW_GAME_PEASHOOTER_STOP: {
        APP_DBG_SIG("ZW_GAME_PEASHOOTER_STOP\n");
        peashooter_moving = PEASHOOTER_MOVE_NONE;
    }
        break;
    default:
        break;
    }
}

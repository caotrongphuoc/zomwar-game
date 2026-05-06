#include "zw_game_gunner.h"

typedef enum {
    GUNNER_MOVE_NONE = 0,
    GUNNER_MOVE_UP,
    GUNNER_MOVE_DOWN,
} gunner_move_dir_t;

zw_game_gunner_t gunner;
static uint32_t gunner_y = AXIS_Y_GUNNER;
static gunner_move_dir_t gunner_moving = GUNNER_MOVE_NONE;

#define ZW_GAME_GUNNER_SETUP() \
do { \
    gunner.x = AXIS_X_GUNNER; \
    gunner.y = AXIS_Y_GUNNER; \
    gunner.visible = WHITE; \
    gunner.action_image = 1; \
    gunner_moving = GUNNER_MOVE_NONE; \
    gunner_y = AXIS_Y_GUNNER; \
} while (0);

#define ZW_GAME_GUNNER_UPDATE() \
do { \
    if (gunner_moving == GUNNER_MOVE_UP) { \
        if (gunner_y > STEP_GUNNER_AXIS_Y + 2) { \
            gunner_y -= STEP_GUNNER_AXIS_Y; \
        } else { \
            gunner_y = 12; \
        } \
    } \
    else if (gunner_moving == GUNNER_MOVE_DOWN) { \
        gunner_y += STEP_GUNNER_AXIS_Y; \
        if (gunner_y > 52) { gunner_y = 52; } \
    } \
    gunner.y = gunner_y; \
    gunner.action_image = 1; \
} while(0);

#define ZW_GAME_GUNNER_RESET() \
do { \
    gunner.x = AXIS_X_GUNNER; \
    gunner.y = AXIS_Y_GUNNER; \
    gunner.visible = BLACK; \
    gunner_y = AXIS_Y_GUNNER; \
    gunner_moving = GUNNER_MOVE_NONE; \
} while(0);

void zw_game_gunner_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_GUNNER_SETUP: {
        APP_DBG_SIG("ZW_GAME_GUNNER_SETUP\n");
        ZW_GAME_GUNNER_SETUP();
    }
        break;

    case ZW_GAME_GUNNER_UPDATE: {
        APP_DBG_SIG("ZW_GAME_GUNNER_UPDATE\n");
        ZW_GAME_GUNNER_UPDATE();
    }
        break;

    case ZW_GAME_GUNNER_UP: {
        APP_DBG_SIG("ZW_GAME_GUNNER_UP\n");
        gunner_moving = GUNNER_MOVE_UP;
    }
        break;

    case ZW_GAME_GUNNER_DOWN: {
        APP_DBG_SIG("ZW_GAME_GUNNER_DOWN\n");
        gunner_moving = GUNNER_MOVE_DOWN;
    }
        break;

    case ZW_GAME_GUNNER_STOP: {
        APP_DBG_SIG("ZW_GAME_GUNNER_STOP\n");
        gunner_moving = GUNNER_MOVE_NONE;
    }
        break;

    case ZW_GAME_GUNNER_RESET: {
        APP_DBG_SIG("ZW_GAME_GUNNER_RESET\n");
        ZW_GAME_GUNNER_RESET();
    }
        break;

    default:
        break;
    }
}

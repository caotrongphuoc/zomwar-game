#include "zw_game_border.h"

#include "zw_game_zombie.h"
#include "zw_game_zomwar.h"
#include "zw_game_car.h"

zw_game_border_t border;
uint32_t zw_game_score = 0;
uint32_t zw_game_kill_count = 0;

#define ZW_GAME_BORDER_SETUP() \
do { \
    border.x = AXIS_X_BORDER; \
    border.visible = WHITE; \
    border.action_image = 0; \
} while (0);

#define ZW_GAME_CHECK_GAME_OVER() \
do { \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (zombie[i].visible != WHITE) continue; \
        if (zombie[i].x <= -(int32_t)ZOMBIE_MIN_LEFT_OFFSET) { \
            uint8_t lane = zombie[i].lane; \
            if (!car[lane].visible) { \
                task_post_pure_msg(ZW_GAME_SCREEN_ID, ZW_GAME_RESET); \
            } \
        } \
    } \
} while(0);

#define ZW_GAME_BORDER_RESET() \
do { \
    border.x = AXIS_X_BORDER; \
    border.visible = BLACK; \
    zw_game_kill_count = 0; \
    zw_game_score = 0; \
} while (0);

void zw_game_border_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_BORDER_SETUP: {
        APP_DBG_SIG("ZW_GAME_BORDER_SETUP\n");
        ZW_GAME_BORDER_SETUP();
    }
        break;

    case ZW_GAME_LEVEL_UP: {
        APP_DBG_SIG("ZW_GAME_LEVEL_UP\n");
    }
        break;

    case ZW_GAME_CHECK_GAME_OVER: {
        APP_DBG_SIG("ZW_GAME_CHECK_GAME_OVER\n");
        ZW_GAME_CHECK_GAME_OVER();
    }
        break;

    case ZW_GAME_ZOMBIE_KILLED: {
        APP_DBG_SIG("ZW_GAME_ZOMBIE_KILLED\n");
        zw_game_score += 10;
        zw_game_kill_count++;
        BUZZER_PlayTones(tones_BUM);
    }
        break;

    case ZW_GAME_BORDER_RESET: {
        APP_DBG_SIG("ZW_GAME_BORDER_RESET\n");
        ZW_GAME_BORDER_RESET();
    }
        break;
    
    default:
        break;
    }
}
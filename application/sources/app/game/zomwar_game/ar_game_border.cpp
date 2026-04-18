#include "ar_game_border.h"

#include "ar_game_zombie.h"
#include "ar_game_zomwar.h"
#include "ar_game_car.h"

ar_game_border_t border;
uint32_t ar_game_score = 0;
uint32_t ar_game_kill_count = 0;

#define AR_GAME_BORDER_SETUP() \
do { \
    border.x = AXIS_X_BORDER; \
    border.visible = WHITE; \
    border.action_image = 0; \
} while (0);

/*#define AR_GAME_LEVEL_UP() \
do { \
     \
} while(0); */

#define AR_GAME_CHECK_GAME_OVER() \
do { \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (zombie[i].visible != WHITE) continue; \
        if (zombie[i].x <= -(int32_t)ZOMBIE_MIN_LEFT_OFFSET) { \
            uint8_t lane = zombie[i].lane; \
            if (!car[lane].visible) { \
                task_post_pure_msg(AR_GAME_SCREEN_ID, AR_GAME_RESET); \
            } \
        } \
    } \
} while(0);

#define AR_GAME_BORDER_RESET() \
do { \
    border.x = AXIS_X_BORDER; \
    border.visible = BLACK; \
    ar_game_kill_count = 0; \
    ar_game_score = 0; \
} while (0);

void ar_game_border_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_BORDER_SETUP: {
        APP_DBG_SIG("AR_GAME_BORDER_SETUP\n");
        AR_GAME_BORDER_SETUP();
    }
        break;

    case AR_GAME_LEVEL_UP: {
        APP_DBG_SIG("AR_GAME_LEVEL_UP\n");
        //AR_GAME_LEVEL_UP();
    }
        break;

    case AR_GAME_CHECK_GAME_OVER: {
        APP_DBG_SIG("AR_GAME_CHECK_GAME_OVER\n");
        AR_GAME_CHECK_GAME_OVER();
    }
        break;

    case AR_GAME_BORDER_RESET: {
        APP_DBG_SIG("AR_GAME_BORDER_RESET\n");
        AR_GAME_BORDER_RESET();
    }
        break;
    
    default:
        break;
    }
}
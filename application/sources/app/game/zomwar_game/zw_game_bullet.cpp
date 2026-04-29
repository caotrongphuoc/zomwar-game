#include "zw_game_bullet.h"
#include "zw_game_peashooter.h"
#include "zw_game_peashooter.h"

zw_game_bullet_t bullet[MAX_NUM_BULLET];

#define ZW_GAME_BULLET_SETUP()  \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        bullet[i].x = 0; \
        bullet[i].y = 0; \
        bullet[i].visible = BLACK; \
        bullet[i].action_image = 1; \
    } \
} while (0);

#define ZW_GAME_BULLET_RUN() \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        if (bullet[i].visible == WHITE) { \
            bullet[i].x += settingsetup.bullet_speed; \
            if (bullet[i].x >= MAX_AXIS_X_BULLET) { \
                bullet[i].visible = BLACK; \
                bullet[i].x = 0; \
            } \
        } \
    } \
} while(0);

#define ZW_GAME_BULLET_SHOOT() \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        if (bullet[i].visible == BLACK) { \
            bullet[i].visible = WHITE; \
            bullet[i].x = peashooter.x + 15; \
            bullet[i].y = peashooter.y -8; \
            peashooter.action_image = 2; \
            BUZZER_PlayTones(tones_cc); \
            break; \
        } \
    } \
} while(0);
#define ZW_GAME_BULLET_RESET() \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        bullet[i].x = 0; \
        bullet[i].y = 0; \
        bullet[i].visible = BLACK; \
        bullet[i].action_image = 1; \
    } \
} while (0);

void zw_game_bullet_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_BULLET_SETUP: {
        APP_DBG_SIG("ZW_GAME_BULLET_SETUP\n");
        ZW_GAME_BULLET_SETUP();
    }
        break;

    case ZW_GAME_BULLET_RUN: {
        APP_DBG_SIG("ZW_GAME_BULLET_RUN\n");
        ZW_GAME_BULLET_RUN();
    }
        break;

    case ZW_GAME_BULLET_SHOOT: {
        APP_DBG_SIG("ZW_GAME_BULLET_SHOOT\n");
        ZW_GAME_BULLET_SHOOT();
    }
        break;

    case ZW_GAME_BULLET_RESET: {
        APP_DBG_SIG("ZW_GAME_BULLET_RESET\n");
        ZW_GAME_BULLET_RESET();
    }
        break;

    default:
        break;
    }
}

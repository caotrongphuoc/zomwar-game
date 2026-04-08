#include "ar_game_bullet.h"

#include "ar_game_zomwar.h"
#include "scr_zomwar_game.h"

ar_game_bullet_t bullet[MAX_NUM_BULLET];

#define AR_GAME_BULLET_SETUP()  \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        bullet[i].x = 0; \
        bullet[i].y = 0; \
        bullet[i].visible = BLACK; \
        bullet[i].action_image = 1; \
    } \
} while (0);

#define AR_GAME_BULLET_RUN() \
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

#define AR_GAME_BULLET_SHOOT() \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        if (bullet[i].visible == BLACK) { \
            bullet[i].visible = WHITE; \
            bullet[i].x = zomwar.x + 10; \
            bullet[i].y = zomwar.y -7; \
            BUZZER_PlayTones(tones_cc); \
            break; \
        } \
    } \
} while(0);
#define AR_GAME_BULLET_RESET() \
do { \
    for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { \
        bullet[i].x = 0; \
        bullet[i].y = 0; \
        bullet[i].visible = BLACK; \
        bullet[i].action_image = 1; \
    } \
} while (0);

void ar_game_bullet_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_BULLET_SETUP: {
        APP_DBG_SIG("AR_GAME_BULLET_SETUP\n");
        AR_GAME_BULLET_SETUP();
    }
        break;

    case AR_GAME_BULLET_RUN: {
        APP_DBG_SIG("AR_GAME_BULLET_RUN\n");
        AR_GAME_BULLET_RUN();
    }
        break;

    case AR_GAME_BULLET_SHOOT: {
        APP_DBG_SIG("AR_GAME_BULLET_SHOOT\n");
        AR_GAME_BULLET_SHOOT();
    }
        break;

    case AR_GAME_BULLET_RESET: {
        APP_DBG_SIG("AR_GAME_BULLET_RESET\n");
        AR_GAME_BULLET_RESET();
    }
        break;

    default:
        break;
    }
}

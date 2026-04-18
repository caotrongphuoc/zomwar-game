#include "ar_game_zombie.h"
#include "ar_game_bullet.h"
#include "ar_game_bang.h"
#include "ar_game_border.h"
#include "scr_zomwar_game.h"

ar_game_zombie_t zombie[NUM_ZOMBIES];
uint32_t last_wave_score = 0;

/* Offset X cua pixel trai ngoai cung theo tung hang va frame:
 * hang 0-9, frame_idx 0 = frame I & III, frame_idx 1 = frame II */
const uint8_t ZOMBIE_LEFT_PX[2][SIZE_BITMAP_ZOMBIES_Y] = {
    {9, 8, 8, 9, 6, 9, 9, 9, 9, 8},  /* frame I & III */
    {9, 8, 8, 9, 7, 9, 9, 9, 9, 9},  /* frame II      */
};

uint8_t wave_warning_timer = 0;
bool wave_warning_active = false;

uint8_t wave_level = 0;
uint8_t level_up_display_timer = 0;

tombstone_t tombstones[NUM_TOMBSTONES];
static uint8_t tombstone_spawn_timer = 0;

static const uint8_t lane_y_arr[NUM_LANES] = LANE_Y;

/* Spawn 1 zombie tại slot i từ bên phải */
static void spawn_zombie(uint8_t i) {
    zombie[i].x = (rand() % 39) + 130;
    zombie[i].y = (rand() % (ZOMBIE_Y_MAX - ZOMBIE_Y_MIN + 1)) + ZOMBIE_Y_MIN;
    zombie[i].visible = WHITE;
    zombie[i].action_image = rand() % 3 + 1;
    zombie[i].dy = 0;
    zombie[i].zigzag_timer = rand() % 10 + 5;
    zombie[i].rising = false;
    zombie[i].rise_ticks = 0;
}

/* Spawn zombie trồi lên từ bia mộ tidx tại slot i */
static void spawn_zombie_from_tombstone(uint8_t i, uint8_t tidx) {
    zombie[i].x = tombstones[tidx].x;
    zombie[i].y = (uint32_t)lane_y_arr[tombstones[tidx].lane] + SIZE_BITMAP_TOMBSTONE_Y;
    zombie[i].visible = WHITE;
    zombie[i].action_image = 1;
    zombie[i].dy = 0;
    zombie[i].zigzag_timer = 0;
    zombie[i].rising = true;
    zombie[i].rise_ticks = ZOMBIE_RISE_TICKS;
}

#define AR_GAME_ZOMBIE_SETUP() \
do { \
    last_wave_score = 0; \
    tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL; \
    wave_level = 0; \
    level_up_display_timer = LEVEL_UP_DISPLAY_TICKS; \
    /* Init zombie pool */ \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (i < NUM_ZOMBIES_INIT) { \
            spawn_zombie(i); \
        } else { \
            zombie[i].visible = BLACK; \
            zombie[i].x = 200; \
            zombie[i].y = ZOMBIE_Y_MIN; \
            zombie[i].dy = 0; \
            zombie[i].zigzag_timer = 0; \
            zombie[i].rising = false; \
            zombie[i].rise_ticks = 0; \
        } \
    } \
    /* Khởi tạo bia mộ: index 0-4 = bia đầu mỗi lane, 5-9 = bia thứ 2 mỗi lane */ \
    /* active phụ thuộc tombstone_lane_1 / tombstone_lane_2 bitmask */ \
    for (uint8_t l = 0; l < NUM_LANES; l++) { \
        tombstones[l].x      = (uint8_t)(rand() % 20 + 65); \
        tombstones[l].lane   = l; \
        tombstones[l].active = (bool)((settingsetup.tombstone_lane_1 >> l) & 1); \
        tombstones[l + 5].x      = (uint8_t)(rand() % 20 + 90); \
        tombstones[l + 5].lane   = l; \
        tombstones[l + 5].active = (bool)((settingsetup.tombstone_lane_2 >> l) & 1); \
    } \
} while (0);

#define AR_GAME_ZOMBIE_RUN() \
do { \
    if (ar_game_state != GAME_PLAY) break; \
    if (level_up_display_timer > 0) level_up_display_timer--; \
    uint8_t active_count = 0; \
    \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (zombie[i].visible != WHITE) continue; \
        active_count++; \
        \
        /* Zombie đang trồi lên từ bia mộ: chỉ di chuyển Y, không X */ \
        if (zombie[i].rising) { \
            if (zombie[i].rise_ticks > 0) { \
                zombie[i].y--; \
                zombie[i].rise_ticks--; \
            } else { \
                zombie[i].rising = false; \
                zombie[i].zigzag_timer = rand() % 10 + 5; \
            } \
            zombie[i].action_image++; \
            if (zombie[i].action_image > 3) zombie[i].action_image = 1; \
            continue; \
        } \
        \
        /* Di chuyển X: dừng khi pixel tay trái chạm border (arm_x = 0) */ \
        if (zombie[i].x - (int32_t)settingsetup.zombie_speed < -(int32_t)ZOMBIE_MIN_LEFT_OFFSET) { \
            zombie[i].x = -(int32_t)ZOMBIE_MIN_LEFT_OFFSET; \
        } else { \
            zombie[i].x -= (int32_t)settingsetup.zombie_speed; \
        } \
        \
        /* Zigzag Y */ \
        if (zombie[i].zigzag_timer > 0) { \
            zombie[i].zigzag_timer--; \
        } else { \
            zombie[i].dy = (int8_t)(rand() % 3) - 1; \
            zombie[i].zigzag_timer = rand() % 10 + 5; \
        } \
        \
        int32_t new_y = (int32_t)zombie[i].y + zombie[i].dy; \
        if (new_y < ZOMBIE_Y_MIN) { new_y = ZOMBIE_Y_MIN; zombie[i].dy = 0; } \
        if (new_y > ZOMBIE_Y_MAX) { new_y = ZOMBIE_Y_MAX; zombie[i].dy = 0; } \
        zombie[i].y = (uint32_t)new_y; \
        \
        /* Animation */ \
        zombie[i].action_image++; \
        if (zombie[i].action_image > 3) zombie[i].action_image = 1; \
    } \
    \
    /* Respawn: luôn giữ ít nhất NUM_ZOMBIES_INIT con */ \
    if (active_count < NUM_ZOMBIES_INIT) { \
        for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
            if (zombie[i].visible != WHITE) { \
                spawn_zombie(i); \
                active_count++; \
                if (active_count >= NUM_ZOMBIES_INIT) break; \
            } \
        } \
    } \
    \
    /* Spawn zombie từ bia mộ theo định kỳ (chỉ nếu có bia active) */ \
    if (tombstone_spawn_timer > 0) { \
        tombstone_spawn_timer--; \
    } else { \
        tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL; \
        uint8_t tidx = (uint8_t)(rand() % NUM_TOMBSTONES); \
        if (tombstones[tidx].active) { \
            for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
                if (zombie[i].visible != WHITE) { \
                    spawn_zombie_from_tombstone(i, tidx); \
                    break; \
                } \
            } \
        } \
    } \
    \
    /* Wave warning + spawn */ \
    if (!wave_warning_active && ar_game_score >= last_wave_score + WAVE_SCORE_INTERVAL) { \
        wave_warning_active = true; \
        wave_warning_timer = WARNING_BLINK_DURATION; \
    } \
    \
    if (wave_warning_active) { \
        if (wave_warning_timer > 0) { \
            wave_warning_timer--; \
        } else { \
            wave_warning_active = false; \
            last_wave_score += WAVE_SCORE_INTERVAL; \
            /* Tăng level và tốc độ zombie mỗi wave */ \
            wave_level++; \
            level_up_display_timer = LEVEL_UP_DISPLAY_TICKS; \
            if (settingsetup.zombie_speed < 6) { \
                settingsetup.zombie_speed++; \
            } \
            uint8_t half = WAVE_SPAWN_COUNT / 2; \
            uint8_t spawned = 0; \
            /* Nửa đầu: spawn từ bia mộ active (mỗi bia 1 con) */ \
            for (uint8_t t = 0; t < NUM_TOMBSTONES && spawned < half; t++) { \
                if (!tombstones[t].active) continue; \
                for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
                    if (zombie[i].visible != WHITE) { \
                        spawn_zombie_from_tombstone(i, t); \
                        spawned++; \
                        break; \
                    } \
                } \
            } \
            /* Nửa sau: spawn từ bên phải */ \
            spawned = 0; \
            for (uint8_t i = 0; i < NUM_ZOMBIES && spawned < half; i++) { \
                if (zombie[i].visible != WHITE) { \
                    spawn_zombie(i); \
                    spawned++; \
                } \
            } \
        } \
    } \
} while(0);

#define AR_GAME_ZOMBIE_DETONATOR() \
do { \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (zombie[i].visible != WHITE) continue; \
        for (uint8_t j = 0; j < MAX_NUM_BULLET; j++) { \
            if (bullet[j].visible != WHITE) continue; \
            \
            /* AABB collision: 2 hình chữ nhật chồng nhau */ \
            /* Bullet box:    [bullet.x, bullet.x + BULLET_W]  x  [bullet.y, bullet.y + BULLET_H] */ \
            /* Zombie box:   [meteor.x, meteor.x + METEOR_W] x [meteor.y, meteor.y + METEOR_H] */ \
            \
            int32_t ax = bullet[j].x; \
            int32_t ay = bullet[j].y; \
            int32_t mx = zombie[i].x; \
            int32_t my = (int32_t)zombie[i].y; \
            \
            /* Kiem tra vien dan co cham bat ky pixel trai ngoai cung nao khong */ \
            uint8_t fidx = (zombie[i].action_image == 2) ? 1 : 0; \
            bool hit = false; \
            for (uint8_t r = 0; r < SIZE_BITMAP_ZOMBIES_Y && !hit; r++) { \
                int32_t lx = mx + (int32_t)ZOMBIE_LEFT_PX[fidx][r]; \
                int32_t ly = my + r; \
                if ((ax <= lx) && (ax + SIZE_BITMAP_BULLET_X > lx) && \
                    (ay <= ly) && (ay + SIZE_BITMAP_BULLET_Y > ly)) { \
                    hit = true; \
                } \
            } \
            \
            if (hit) { \
                int32_t dead_x = zombie[i].x; \
                uint32_t dead_y = zombie[i].y; \
                \
                zombie[i].visible = BLACK; \
                zombie[i].x = 200; \
                bullet[j].visible = BLACK; \
                bullet[j].x = 0; \
                \
                for (uint8_t k = 0; k < NUM_BANG; k++) { \
                    if (bang[k].visible == BLACK) { \
                        bang[k].visible = WHITE; \
                        bang[k].x = (dead_x > 5) ? (uint32_t)(dead_x - 5) : 0; \
                        bang[k].y = dead_y + 2; \
                        bang[k].action_image = 1; \
                        break; \
                    } \
                } \
                ar_game_score += 10; \
                ar_game_kill_count++; \
                BUZZER_PlayTones(tones_BUM); \
                break; \
            } \
        } \
    } \
} while(0);


#define AR_GAME_ZOMBIE_RESET() \
do { \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        zombie[i].visible = BLACK; \
        zombie[i].x = 200; \
        zombie[i].dy = 0; \
        zombie[i].rising = false; \
        zombie[i].rise_ticks = 0; \
    } \
    last_wave_score = 0; \
    wave_warning_timer = 0; \
    wave_warning_active = false; \
    tombstone_spawn_timer = 0; \
    wave_level = 0; \
    level_up_display_timer = 0; \
} while (0);

void ar_game_zombie_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case AR_GAME_ZOMBIE_SETUP: {
        APP_DBG_SIG("AR_GAME_ZOMBIE_SETUP\n");
        AR_GAME_ZOMBIE_SETUP();
    }
        break;
    case AR_GAME_ZOMBIE_RUN: {
        APP_DBG_SIG("AR_GAME_ZOMBIE_RUN\n");
        AR_GAME_ZOMBIE_RUN();
    }
        break;
    case AR_GAME_ZOMBIE_DETONATOR: {
        APP_DBG_SIG("AR_GAME_ZOMBIE_DETONATOR\n");
        AR_GAME_ZOMBIE_DETONATOR();
    }
        break;
    case AR_GAME_ZOMBIE_RESET: {
        APP_DBG_SIG("AR_GAME_ZOMBIE_RESET\n");
        AR_GAME_ZOMBIE_RESET();
    }
        break;
    default:
        break;
    }
}

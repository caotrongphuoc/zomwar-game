#include "zw_game_zombie.h"

#include "zw_game_bullet.h"
#include "zw_game_bang.h"
#include "zw_game_border.h"

zw_game_zombie_t zombie[NUM_ZOMBIES];
uint32_t last_wave_score = 0;
static bool game_active = false;

const uint8_t ZOMBIE_LEFT_PX[2][SIZE_BITMAP_ZOMBIES_Y] = {
    {9, 8, 8, 9, 6, 9, 9, 9, 9, 8},
    {9, 8, 8, 9, 7, 9, 9, 9, 9, 9},
};

uint8_t wave_warning_timer = 0;
bool wave_warning_active = false;

uint8_t wave_level = 0;

tombstone_t tombstones[NUM_TOMBSTONES];
static uint8_t tombstone_spawn_timer = 0;

static const uint8_t lane_y_arr[NUM_LANES] = LANE_Y;

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

#define ZW_GAME_ZOMBIE_SETUP() \
do { \
    game_active = true; \
    last_wave_score = 0; \
    tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL; \
    wave_level = 0; \
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
    for (uint8_t l = 0; l < NUM_LANES; l++) { \
        tombstones[l].x      = (uint8_t)(rand() % 20 + 65); \
        tombstones[l].lane   = l; \
        tombstones[l].active = (bool)((settingsetup.tombstone_lane_1 >> l) & 1); \
        tombstones[l + 5].x      = (uint8_t)(rand() % 20 + 90); \
        tombstones[l + 5].lane   = l; \
        tombstones[l + 5].active = (bool)((settingsetup.tombstone_lane_2 >> l) & 1); \
    } \
} while (0);

#define ZW_GAME_ZOMBIE_RUN() \
do { \
    if (!game_active) break; \
    uint8_t active_count = 0; \
    \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (zombie[i].visible != WHITE) continue; \
        active_count++; \
        \
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
        if (zombie[i].x - (int32_t)settingsetup.zombie_speed < -(int32_t)ZOMBIE_MIN_LEFT_OFFSET) { \
            zombie[i].x = -(int32_t)ZOMBIE_MIN_LEFT_OFFSET; \
        } else { \
            zombie[i].x -= (int32_t)settingsetup.zombie_speed; \
        } \
        \
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
        zombie[i].action_image++; \
        if (zombie[i].action_image > 3) zombie[i].action_image = 1; \
    } \
    \
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
    if (!wave_warning_active && zw_game_score >= last_wave_score + WAVE_SCORE_INTERVAL) { \
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
            wave_level++; \
            task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_LEVEL_UP); \
            uint8_t half = WAVE_SPAWN_COUNT / 2; \
            uint8_t spawned = 0; \
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

#define ZW_GAME_ZOMBIE_DETONATOR() \
do { \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (zombie[i].visible != WHITE) continue; \
        for (uint8_t j = 0; j < MAX_NUM_BULLET; j++) { \
            if (bullet[j].visible != WHITE) continue; \
            \
            int32_t ax = bullet[j].x; \
            int32_t ay = bullet[j].y; \
            int32_t mx = zombie[i].x; \
            int32_t my = (int32_t)zombie[i].y; \
            \
            bool hit = (ax + SIZE_BITMAP_BULLET_X > mx + 12) && \
                       (ax < mx + (int32_t)SIZE_BITMAP_ZOMBIES_X) && \
                       (ay + (int32_t)SIZE_BITMAP_BULLET_Y > my) && \
                       (ay < my + (int32_t)SIZE_BITMAP_ZOMBIES_Y); \
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
                { \
                    uint8_t bk = bang_alloc_slot(); \
                    bang[bk].visible = WHITE; \
                    bang[bk].x = (dead_x > 5) ? (uint32_t)(dead_x - 5) : 0; \
                    bang[bk].y = dead_y + 2; \
                    bang[bk].action_image = 1; \
                } \
                task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_ZOMBIE_KILLED); \
                break; \
            } \
        } \
    } \
} while(0);

#define ZW_GAME_ZOMBIE_RESET() \
do { \
    game_active = false; \
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
} while (0);

void zw_game_zombie_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_ZOMBIE_SETUP: {
        APP_DBG_SIG("ZW_GAME_ZOMBIE_SETUP\n");
        ZW_GAME_ZOMBIE_SETUP();
    }
        break;
    case ZW_GAME_ZOMBIE_RUN: {
        APP_DBG_SIG("ZW_GAME_ZOMBIE_RUN\n");
        ZW_GAME_ZOMBIE_RUN();
    }
        break;
    case ZW_GAME_ZOMBIE_DETONATOR: {
        APP_DBG_SIG("ZW_GAME_ZOMBIE_DETONATOR\n");
        ZW_GAME_ZOMBIE_DETONATOR();
    }
        break;
    case ZW_GAME_ZOMBIE_RESET: {
        APP_DBG_SIG("ZW_GAME_ZOMBIE_RESET\n");
        ZW_GAME_ZOMBIE_RESET();
    }
        break;
    default:
        break;
    }
}

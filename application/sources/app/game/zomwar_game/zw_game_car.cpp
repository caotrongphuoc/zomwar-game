#include "zw_game_car.h"
#include "zw_game_zombie.h"
#include "zw_game_bang.h"

zw_game_car_t car[NUM_LANES];
static const int32_t lane_y[NUM_LANES] = {2, 12, 22, 32, 42};
static bool game_active = false;

#define ZW_GAME_CAR_SETUP() \
do { \
    game_active = true; \
    for (uint8_t i = 0; i < NUM_LANES; i++) { \
        car[i].x       = AXIS_X_CAR; \
        car[i].y       = lane_y[i]; \
        car[i].lane    = i; \
        car[i].visible = (settingsetup.num_car >> i) & 1; \
        car[i].running = false; \
        car[i].action_image = 1; \
    } \
} while(0);

/* Tìm car gần nhất với zombie theo Y */
static int8_t find_nearest_mower(uint32_t zy) {
    int8_t best = -1;
    int32_t best_dist = 999;
    for (uint8_t i = 0; i < NUM_LANES; i++) {
        if (!car[i].visible || car[i].running) continue;
        int32_t dist = (int32_t)zy - (int32_t)car[i].y;
        if (dist < 0) dist = -dist;
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    /* Chỉ kích hoạt nếu trong tầm hit */
    if (best >= 0 && best_dist > CAR_HIT_RANGE_Y) {
        best = -1;
    }
    return best;
}

#define ZW_GAME_CAR_RUN() \
do { \
    if (!game_active) break; \
    \
    bool game_over = false; \
    \
    for (uint8_t i = 0; i < NUM_ZOMBIES; i++) { \
        if (game_over) break; \
        if (zombie[i].visible != WHITE) continue; \
        \
        if (zombie[i].x <= -(int32_t)ZOMBIE_MIN_LEFT_OFFSET) { \
            int8_t m = find_nearest_mower(zombie[i].y); \
            if (m >= 0) { \
                car[m].running = true; \
            } else { \
                game_over = true; \
                task_post_pure_msg(ZW_GAME_SCREEN_ID, ZW_GAME_RESET); \
            } \
            zombie[i].visible = BLACK; \
            zombie[i].x = 200; \
            continue; \
        } \
        \
        for (uint8_t m = 0; m < NUM_LANES; m++) { \
            if (!car[m].visible || car[m].running) continue; \
            int32_t dy = (int32_t)zombie[i].y - (int32_t)car[m].y; \
            if (dy < 0) dy = -dy; \
            if (dy <= CAR_HIT_RANGE_Y) { \
                /* Kich hoat xe khi pixel tay zombie cham vao xe */ \
                if (zombie[i].x + (int32_t)ZOMBIE_MIN_LEFT_OFFSET <= (int32_t)(car[m].x + SIZE_BITMAP_CAR_X)) { \
                    car[m].running = true; \
                } \
            } \
        } \
    } \
    \
    if (game_over) break; \
    \
    for (uint8_t i = 0; i < NUM_LANES; i++) { \
        if (!car[i].visible || !car[i].running) continue; \
        car[i].x += CAR_SPEED; \
        car[i].action_image++; \
        if (car[i].action_image > 3) car[i].action_image = 1; \
        \
        for (uint8_t j = 0; j < NUM_ZOMBIES; j++) { \
            if (zombie[j].visible != WHITE) continue; \
            int32_t dy = (int32_t)zombie[j].y - (int32_t)car[i].y; \
            if (dy < 0) dy = -dy; \
            if (dy > CAR_HIT_RANGE_Y) continue; \
            /* Xe can zombie khi bat ky pixel trai ngoai cung nao cua zombie nam trong vung xe */ \
            uint8_t fidx_c = (zombie[j].action_image == 2) ? 1 : 0; \
            bool overlap_x = false; \
            for (uint8_t r = 0; r < SIZE_BITMAP_ZOMBIES_Y && !overlap_x; r++) { \
                int32_t lx = zombie[j].x + (int32_t)ZOMBIE_LEFT_PX[fidx_c][r]; \
                if (lx >= (int32_t)car[i].x && lx < (int32_t)(car[i].x + SIZE_BITMAP_CAR_X)) { \
                    overlap_x = true; \
                } \
            } \
            if (overlap_x) { \
                zombie[j].visible = BLACK; \
                zombie[j].x = 200; \
                for (uint8_t k = 0; k < NUM_BANG; k++) { \
                    if (bang[k].visible == BLACK) { \
                        bang[k].visible      = WHITE; \
                        bang[k].x            = car[i].x; \
                        bang[k].y            = car[i].y; \
                        bang[k].action_image = 1; \
                        task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_ZOMBIE_KILLED); \
                        break; \
                    } \
                } \
            } \
        } \
        \
        if (car[i].x > LCD_WIDTH) { \
            car[i].visible = false; \
            car[i].running = false; \
        } \
    } \
} while(0);

#define ZW_GAME_CAR_RESET() \
do { \
    game_active = false; \
    for (uint8_t i = 0; i < NUM_LANES; i++) { \
        car[i].x       = AXIS_X_CAR; \
        car[i].y       = lane_y[i]; \
        car[i].visible = false; \
        car[i].running = false; \
    } \
} while(0);

void zw_game_car_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_CAR_SETUP: {
        ZW_GAME_CAR_SETUP();
    }
        break;
    case ZW_GAME_CAR_RUN: {
        ZW_GAME_CAR_RUN();
    }
        break;
    case ZW_GAME_CAR_RESET: {
        ZW_GAME_CAR_RESET();
    }
        break;
    default:
        break;
    }
}

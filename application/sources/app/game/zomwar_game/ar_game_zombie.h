#ifndef __AR_GAME_ZOMBIE_H__
#define __AR_GAME_ZOMBIE_H__
#include <stdio.h>
#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"
#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "buzzer.h"
#include "scr_zomwar_game.h"

#define NUM_ZOMBIES              (100)
#define NUM_ZOMBIES_INIT         (5)     // Số zombie ban đầu
#define WAVE_SPAWN_COUNT            (10)    // Số zombie mỗi wave
#define WAVE_SCORE_INTERVAL         (250)   // Mỗi 200 điểm spawn wave

#define SIZE_BITMAP_ZOMBIES_X    (25)
#define SIZE_BITMAP_ZOMBIES_Y    (10)

/* Offset X cua pixel trai ngoai cung theo tung hang (0..9), theo frame animation.
 * frame_idx 0 = frame I & III, frame_idx 1 = frame II */
#define ZOMBIE_MIN_LEFT_OFFSET   (6)   /* gia tri nho nhat: dau tay frame I/III, hang 4 */
extern const uint8_t ZOMBIE_LEFT_PX[2][SIZE_BITMAP_ZOMBIES_Y];

#define ZOMBIE_Y_MIN             (2)
#define ZOMBIE_Y_MAX             (42)

#define WARNING_BLINK_DURATION  (30)   // 3 giây ÷ 100ms/tick = 30 tick
#define WARNING_BLINK_RATE      (3)    // chớp mỗi 3 tick (300ms)

#define SIZE_BITMAP_WARNING_X   (16)
#define SIZE_BITMAP_WARNING_Y   (14)

typedef struct {
    int32_t x;
    uint32_t y;
    int8_t dy;              // hướng zigzag: -1, 0, +1
    uint8_t zigzag_timer;   // đếm tick để đổi hướng
    uint8_t action_image;
    uint8_t lane;           // giữ lại cho car tương thích
    bool visible;
    bool rising;            // đang trồi lên từ bia mộ
    uint8_t rise_ticks;     // số tick còn lại để trồi lên
} ar_game_zombie_t;

#define TOMBSTONES_PER_LANE     (2)
#define NUM_TOMBSTONES          (10)    /* 5 lanes x 2 per lane */
#define SIZE_BITMAP_TOMBSTONE_X (8)
#define SIZE_BITMAP_TOMBSTONE_Y (10)
#define TOMBSTONE_SPAWN_INTERVAL (35)   // cứ 35 tick (~3.5s) thử spawn từ bia mộ
#define ZOMBIE_RISE_TICKS        (10)   // 10 tick = 1s để trồi lên hoàn toàn

typedef struct {
    uint8_t x;
    uint8_t lane;
    bool active;
} tombstone_t;

extern tombstone_t tombstones[NUM_TOMBSTONES];

extern ar_game_zombie_t zombie[NUM_ZOMBIES];
extern uint32_t last_wave_score;
extern uint8_t wave_warning_timer;     // đếm ngược 30 tick
extern bool wave_warning_active;

#define LEVEL_UP_DISPLAY_TICKS  (20)   // 2 giây (20 tick × 100ms)
extern uint8_t wave_level;             // level hiện tại (0 = LV0 ban đầu)
extern uint8_t level_up_display_timer; // đếm ngược để hiển thị thông báo LV
#endif

#ifndef __ZW_GAME_ZOMBIE_H__
#define __ZW_GAME_ZOMBIE_H__

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

#define NUM_ZOMBIES              (50)
#define NUM_ZOMBIES_INIT         (5)    
#define WAVE_SPAWN_COUNT         (8)    
#define WAVE_SCORE_INTERVAL      (200)  
#define ZOMBIE_SPEED_MAX         (6)  

#define SIZE_BITMAP_ZOMBIES_X    (25)
#define SIZE_BITMAP_ZOMBIES_Y    (10)
#define ZOMBIE_MIN_LEFT_OFFSET   (6)   

#define ZOMBIE_Y_MIN             (2)
#define ZOMBIE_Y_MAX             (42)

#define WARNING_BLINK_DURATION   (30)   
#define WARNING_BLINK_RATE       (3)  

#define SIZE_BITMAP_WARNING_X    (16)
#define SIZE_BITMAP_WARNING_Y    (14)

#define TOMBSTONES_PER_LANE     (2)
#define NUM_TOMBSTONES          (10)   
#define SIZE_BITMAP_TOMBSTONE_X (8)
#define SIZE_BITMAP_TOMBSTONE_Y (10)
#define TOMBSTONE_SPAWN_INTERVAL (35)  
#define ZOMBIE_RISE_TICKS        (10)  

typedef struct {
    int32_t x;
    uint32_t y;
    int8_t dy;            
    uint8_t zigzag_timer;   
    uint8_t action_image;
    bool visible;
    bool rising;          
    uint8_t rise_ticks;   
} zw_game_zombie_t;

typedef struct {
    uint8_t x;
    uint8_t lane;
    bool active;
} tombstone_t;

extern tombstone_t tombstones[NUM_TOMBSTONES];
extern const uint8_t ZOMBIE_LEFT_PX[2][SIZE_BITMAP_ZOMBIES_Y];
extern zw_game_zombie_t zombie[NUM_ZOMBIES];
extern uint32_t last_wave_score;
extern uint8_t wave_warning_timer;    
extern bool wave_warning_active;
extern uint8_t wave_level;

#endif //__ZW_GAME_ZOMBIE_H__

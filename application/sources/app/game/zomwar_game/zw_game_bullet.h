#ifndef __ZW_GAME_BULLET_H__
#define __ZW_GAME_BULLET_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "buzzer.h"
#include "scr_zomwar_game.h"

#define MAX_NUM_BULLET       (30)
#define MAX_AXIS_X_BULLET    (128)
#define SIZE_BITMAP_BULLET_X (5)
#define SIZE_BITMAP_BULLET_Y (5)

typedef struct {
    bool visible;
    uint32_t x, y;
    uint8_t action_image;
} zw_game_bullet_t;

extern zw_game_bullet_t bullet[MAX_NUM_BULLET];

#endif //__ZW_GAME_BULLET_H__

#ifndef __ZW_GAME_GUNNER_H__
#define __ZW_GAME_GUNNER_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "scr_zomwar_game.h"

#define STEP_GUNNER_AXIS_Y     (10)
#define SIZE_BITMAP_GUNNER_X   (25)
#define SIZE_BITMAP_GUNNER_Y   (10)
#define AXIS_X_GUNNER          (14)
#define AXIS_Y_GUNNER          (32)

typedef struct {
    bool visible;
    uint32_t x, y;
    uint8_t action_image;
} zw_game_gunner_t;

extern zw_game_gunner_t gunner;


#endif //__ZW_GAME_GUNNER_H__

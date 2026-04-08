#ifndef __AR_GAME_ZOMWAR_H__
#define __AR_GAME_ZOMWAR_H__

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

#define STEP_ZOMWAR_AXIS_Y     (10)
#define SIZE_BITMAP_ZOMWAR_X   (25)
#define SIZE_BITMAP_ZOMWAR_Y   (10)
#define AXIS_X_ZOMWAR          (10)
#define AXIS_Y_ZOMWAR          (30)

typedef struct {
    bool visible;
    uint32_t x, y;
    uint8_t action_image;
} ar_game_zomwar_t;

extern ar_game_zomwar_t zomwar;

typedef enum {
    ZOMWAR_MOVE_NONE = 0,
    ZOMWAR_MOVE_UP,
    ZOMWAR_MOVE_DOWN,
} zomwar_move_dir_t;

extern volatile zomwar_move_dir_t zomwar_moving;

#endif //__AR_GAME_ZOMWAR_H__

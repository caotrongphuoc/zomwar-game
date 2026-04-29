#ifndef __SCR_TOMBSTONE_COUNT_H__
#define __SCR_TOMBSTONE_COUNT_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "buzzer.h"

#include "eeprom.h"
#include "app_eeprom.h"

#include "scr_bitmap.h"

#define TB_COUNT_NUM_LANES          (5)
#define TB_COUNT_STEP_CHOSSE        (10)
// items (5 lanes + EXIT)
#define TB_COUNT_ITEM_ARRDESS_0     (0)
#define TB_COUNT_ITEM_ARRDESS_1     (TB_COUNT_STEP_CHOSSE)
#define TB_COUNT_ITEM_ARRDESS_2     (TB_COUNT_STEP_CHOSSE*2)
#define TB_COUNT_ITEM_ARRDESS_3     (TB_COUNT_STEP_CHOSSE*3)
#define TB_COUNT_ITEM_ARRDESS_4     (TB_COUNT_STEP_CHOSSE*4)
#define TB_COUNT_ITEM_ARRDESS_5     (TB_COUNT_STEP_CHOSSE*5)
#define TB_COUNT_ITEM_ARRDESS_6     (TB_COUNT_STEP_CHOSSE*6)
// Text and Number
#define TB_COUNT_TEXT_AXIS_X        (20)
#define TB_COUNT_NUMBER_AXIS_X      (110)
// Chosse icon
#define TB_COUNT_CHOSSE_ICON_AXIS_Y (14)
#define TB_COUNT_CHOSSE_ICON_SIZE_W (20)
#define TB_COUNT_CHOSSE_ICON_SIZE_H (20)
// Frames
#define TB_COUNT_FRAMES_AXIS_X      (0)
#define TB_COUNT_FRAMES_AXIS_Y_1    (2)
#define TB_COUNT_FRAMES_STEP        (10)
#define TB_COUNT_FRAMES_SIZE_W      (123)
#define TB_COUNT_FRAMES_SIZE_H      (9)
#define TB_COUNT_FRAMES_SIZE_R      (2)

extern view_dynamic_t dyn_view_item_tombstone_count;
extern view_screen_t scr_tombstone_count;
extern void scr_tombstone_count_handle(ak_msg_t* msg);

#endif //__SCR_TOMBSTONE_COUNT_H__

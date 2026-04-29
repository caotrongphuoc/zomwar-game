#ifndef __SCR_CAR_POSITION_H__
#define __SCR_CAR_POSITION_H__

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

#define CAR_POS_NUM_CARS                (5)
#define CAR_POS_STEP_CHOSSE             (10)
// items (5 cars + EXIT)
#define CAR_POS_ITEM_ARRDESS_0          (0)
#define CAR_POS_ITEM_ARRDESS_1          (CAR_POS_STEP_CHOSSE)
#define CAR_POS_ITEM_ARRDESS_2          (CAR_POS_STEP_CHOSSE*2)
#define CAR_POS_ITEM_ARRDESS_3          (CAR_POS_STEP_CHOSSE*3)
#define CAR_POS_ITEM_ARRDESS_4          (CAR_POS_STEP_CHOSSE*4)
#define CAR_POS_ITEM_ARRDESS_5          (CAR_POS_STEP_CHOSSE*5)
#define CAR_POS_ITEM_ARRDESS_6          (CAR_POS_STEP_CHOSSE*6)
// Text and Number
#define CAR_POS_TEXT_AXIS_X             (0)
#define CAR_POS_NUMBER_AXIS_X           (110)
// Chosse icon
#define CAR_POS_CHOSSE_ICON_AXIS_Y      (14)
#define CAR_POS_CHOSSE_ICON_SIZE_W      (20)
#define CAR_POS_CHOSSE_ICON_SIZE_H      (20)
// Frames
#define CAR_POS_FRAMES_AXIS_X           (0)
#define CAR_POS_FRAMES_AXIS_Y_1         (2)
#define CAR_POS_FRAMES_STEP             (10)
#define CAR_POS_FRAMES_SIZE_W           (123)
#define CAR_POS_FRAMES_SIZE_H           (9)
#define CAR_POS_FRAMES_SIZE_R           (2)

extern view_dynamic_t dyn_view_item_car_position;
extern view_screen_t scr_car_position;
extern void scr_car_position_handle(ak_msg_t* msg);

#endif //__SCR_CAR_POSITION_H__

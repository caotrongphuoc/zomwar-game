#ifndef __SCR_ZOMWAR_GAME_H__
#define __SCR_ZOMWAR_GAME_H__

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

#include "eeprom.h"
#include "app_eeprom.h"

#include "buzzer.h"

#include "screens.h"
#include "screens_bitmap.h"

#include "ar_game_zomwar.h"
#include "ar_game_bullet.h"
#include "ar_game_bang.h"
#include "ar_game_border.h"
#include "ar_game_car.h"
#include "ar_game_zombie.h"

#define GAME_OFF		(0)
#define GAME_PLAY		(1)
#define GAME_OVER		(2)


extern uint8_t ar_game_state;
extern ar_game_setting_t settingsetup;

extern view_dynamic_t dyn_view_item_zomwar_game;
extern view_screen_t scr_zomwar_game;
extern void scr_zomwar_game_handle(ak_msg_t* msg);

#endif //__SCR_ZOMWAR_GAME_H__

#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Setting game */
/*****************************************************************************/
zw_game_setting_t settingdata;
static uint8_t setting_location_chosse;

/*****************************************************************************/
/* View - Setting game */
/*****************************************************************************/
static void view_scr_game_setting();

view_dynamic_t dyn_view_item_game_setting = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_setting
};

view_screen_t scr_game_setting = {
	&dyn_view_item_game_setting,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_setting() {
	view_render.setTextSize(1);

	uint8_t sel = (setting_location_chosse / STEP_SETTING_CHOSSE) - 1;

	for (uint8_t f = 0; f < 5; f++) {
		uint8_t frame_y = ZW_GAME_SETTING_FRAMES_AXIS_Y_1 + ZW_GAME_SETTING_FRAMES_STEP * f;
		bool selected  = (f == sel);
		uint8_t fg     = selected ? BLACK : WHITE;

		if (selected) {
			view_render.fillRoundRect(
				ZW_GAME_SETTING_FRAMES_AXIS_X, frame_y,
				ZW_GAME_SETTING_FRAMES_SIZE_W, ZW_GAME_SETTING_FRAMES_SIZE_H,
				ZW_GAME_SETTING_FRAMES_SIZE_R, WHITE);
		} else {
			view_render.drawRoundRect(
				ZW_GAME_SETTING_FRAMES_AXIS_X, frame_y,
				ZW_GAME_SETTING_FRAMES_SIZE_W, ZW_GAME_SETTING_FRAMES_SIZE_H,
				ZW_GAME_SETTING_FRAMES_SIZE_R, WHITE);
		}

		view_render.setTextColor(fg);
		uint8_t text_y = frame_y + 2;

		switch (f) {
		case 0: {
			uint8_t car_count = 0;
			for (uint8_t i = 0; i < 5; i++) {
				if ((settingdata.num_car >> i) & 1) car_count++;
			}
			view_render.setCursor(2, text_y);
			view_render.print("Cars");
			view_render.setCursor(104, text_y);
			view_render.print("[");
			view_render.print(car_count);
			view_render.print("]");
		} break;
		case 1: {
			uint8_t total_t = 0;
			for (uint8_t i = 0; i < 5; i++) {
				if ((settingdata.tombstone_lane_1 >> i) & 1) total_t++;
				if ((settingdata.tombstone_lane_2 >> i) & 1) total_t++;
			}
			view_render.setCursor(2, text_y);
			view_render.print("Tombstones");
			view_render.setCursor(total_t >= 10 ? 98 : 104, text_y);
			view_render.print("[");
			view_render.print(total_t);
			view_render.print("]");
		} break;
		case 2:
			view_render.setCursor(2, text_y);
			view_render.print("Zombies speed");
			view_render.setCursor(104, text_y);
			view_render.print("[");
			view_render.print(settingdata.zombie_speed);
			view_render.print("]");
			break;
		case 3:
			view_render.setCursor(2, text_y);
			view_render.print("Sound");
			view_render.drawBitmap(110, text_y, settingdata.silent ? speaker_2 : speaker_1, 7, 7, fg);
			break;
		case 4:
			view_render.setCursor(45, text_y);
			view_render.print(" EXIT ");
			break;
		}
	}

	view_render.setTextColor(WHITE);
	view_render.update();
}

/*****************************************************************************/
/* Handle - Setting game */
/*****************************************************************************/
void scr_game_setting_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.clear();
		setting_location_chosse = SETTING_ITEM_ARRDESS_1;
		eeprom_read(	EEPROM_SETTING_START_ADDR,
						(uint8_t*)&settingdata,
						sizeof(settingdata));
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		switch (setting_location_chosse) {
		case SETTING_ITEM_ARRDESS_1: {
			SCREEN_TRAN(scr_car_position_handle, &scr_car_position);
			BUZZER_PlayTones(tones_cc);
		}
			break;

		case SETTING_ITEM_ARRDESS_2: {
			SCREEN_TRAN(scr_tombstone_count_handle, &scr_tombstone_count);
			BUZZER_PlayTones(tones_cc);
		}
			break;

		case SETTING_ITEM_ARRDESS_3: {
			settingdata.zombie_speed++;
			if (settingdata.zombie_speed > 5) {
				settingdata.zombie_speed = 1;
			}
			BUZZER_PlayTones(tones_cc);
		}
			break;

		case SETTING_ITEM_ARRDESS_4: {
			settingdata.silent = !settingdata.silent;
			BUZZER_Sleep(settingdata.silent);
			BUZZER_PlayTones(tones_cc);
		}
			break;

		case SETTING_ITEM_ARRDESS_5: {
			eeprom_write(	EEPROM_SETTING_START_ADDR,
							(uint8_t*)&settingdata,
							sizeof(settingdata));
			SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
			BUZZER_PlayTones(tones_startup);
		}
			break;

		default:
			break;
		}
	}
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		setting_location_chosse -= STEP_SETTING_CHOSSE;
		if (setting_location_chosse == SETTING_ITEM_ARRDESS_0) {
			setting_location_chosse = SETTING_ITEM_ARRDESS_5;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		setting_location_chosse += STEP_SETTING_CHOSSE;
		if (setting_location_chosse > SETTING_ITEM_ARRDESS_5) {
			setting_location_chosse = SETTING_ITEM_ARRDESS_1;
		}
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}

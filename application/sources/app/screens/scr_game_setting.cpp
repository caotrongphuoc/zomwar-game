#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Setting game */
/*****************************************************************************/
ar_game_setting_t settingdata;
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
	view_render.setTextColor(WHITE);

	// Choice icon
	view_render.drawBitmap(	0,
							setting_location_chosse -
							AR_GAME_SETTING_CHOSSE_ICON_AXIS_Y,
							chosse_icon,
							AR_GAME_SETTING_CHOSSE_ICON_SIZE_W,
							AR_GAME_SETTING_CHOSSE_ICON_SIZE_H,
							WHITE);

	// Speaker icon (Sound item = frame 4, Y=38+4=42)
	if (settingdata.silent == 0) {
		view_render.drawBitmap(109, 40, speaker_1, 7, 7, WHITE);
	} else {
		view_render.drawBitmap(109, 40, speaker_2, 7, 7, WHITE);
	}

	// 5 frames
	for (uint8_t f = 0; f < 5; f++) {
		view_render.drawRoundRect(
			AR_GAME_SETTING_FRAMES_AXIS_X,
			AR_GAME_SETTING_FRAMES_AXIS_Y_1 + AR_GAME_SETTING_FRAMES_STEP * f,
			AR_GAME_SETTING_FRAMES_SIZE_W,
			AR_GAME_SETTING_FRAMES_SIZE_H,
			AR_GAME_SETTING_FRAMES_SIZE_R,
			WHITE);
	}

	// Item 1: Cars
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 4);
	view_render.print(" Cars        [  ]");
	view_render.setCursor(AR_GAME_SETTING_NUMBER_AXIS_X, 4);
	uint8_t car_count = 0;
	for (uint8_t i = 0; i < 5; i++) {
		if ((settingdata.num_car >> i) & 1) car_count++;
	}
	view_render.print(car_count);

	// Item 2: Tombstones - hiện tổng số bia mộ
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 16);
	view_render.print(" Tombstones  [  ]");
	view_render.setCursor(AR_GAME_SETTING_NUMBER_AXIS_X, 16);
	uint8_t total_t = 0;
	for (uint8_t i = 0; i < 5; i++) {
		if ((settingdata.tombstone_lane_1 >> i) & 1) total_t++;
		if ((settingdata.tombstone_lane_2 >> i) & 1) total_t++;
	}
	view_render.print(total_t);

	// Item 3: Zombie speed
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 28);
	view_render.print(" Zombies sp  [  ]");
	view_render.setCursor(AR_GAME_SETTING_NUMBER_AXIS_X, 28);
	view_render.print(settingdata.zombie_speed);

	// Item 4: Sound
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X, 40);
	view_render.print(" Sound          ");

	// Item 5: EXIT
	view_render.setCursor(AR_GAME_SETTING_TEXT_AXIS_X + 32, 53);
	view_render.print(" EXIT ");

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
			// Vào sub-menu chọn vị trí xe
			SCREEN_TRAN(scr_car_position_handle, &scr_car_position);
		}
			break;

		case SETTING_ITEM_ARRDESS_2: {
			// Vào sub-menu chỉnh bia mộ theo từng lane
			SCREEN_TRAN(scr_tombstone_count_handle, &scr_tombstone_count);
		}
			break;

		case SETTING_ITEM_ARRDESS_3: {
			// Cycle zombie speed 1-5
			settingdata.zombie_speed++;
			if (settingdata.zombie_speed > 5) {
				settingdata.zombie_speed = 1;
			}
		}
			break;

		case SETTING_ITEM_ARRDESS_4: {
			// Toggle sound
			settingdata.silent = !settingdata.silent;
			BUZZER_Sleep(settingdata.silent);
		}
			break;

		case SETTING_ITEM_ARRDESS_5: {
			// Save and exit
			settingdata.bullet_speed = 5;
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
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_LONG_PRESSED\n");
		// Reset về max
		settingdata.num_car = 0x1F;
		settingdata.tombstone_lane_1 = 0x1F;
		settingdata.tombstone_lane_2 = 0x1F;
		settingdata.zombie_speed = 5;
		settingdata.silent = 0;
	}
		BUZZER_Sleep(settingdata.silent);
		BUZZER_PlayTones(tones_cc);
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

	case AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_LONG_PRESSED\n");
		// Reset về min
		settingdata.num_car = 0x00;
		settingdata.tombstone_lane_1 = 0x00;
		settingdata.tombstone_lane_2 = 0x00;
		settingdata.zombie_speed = 1;
		settingdata.silent = 1;
	}
		BUZZER_Sleep(settingdata.silent);
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

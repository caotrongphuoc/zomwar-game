#include "scr_car_position.h"
#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Car position selection */
/*****************************************************************************/
static uint8_t car_pos_location_chosse;

/*****************************************************************************/
/* View - Car position selection */
/*****************************************************************************/
static void view_scr_car_position();

view_dynamic_t dyn_view_item_car_position = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_car_position
};

view_screen_t scr_car_position = {
	&dyn_view_item_car_position,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

static void view_scr_car_position() {
	view_render.setTextSize(1);

	// Item dang duoc chon (0-based): 0..CAR_POS_NUM_CARS (EXIT)
	uint8_t sel = (car_pos_location_chosse / CAR_POS_STEP_CHOSSE) - 1;

	for (uint8_t i = 0; i <= CAR_POS_NUM_CARS; i++) {
		uint8_t frame_y  = CAR_POS_FRAMES_AXIS_Y_1 + CAR_POS_FRAMES_STEP * i;
		bool selected    = (i == sel);
		uint8_t fg       = selected ? BLACK : WHITE;

		if (selected) {
			view_render.fillRoundRect(CAR_POS_FRAMES_AXIS_X, frame_y,
									  CAR_POS_FRAMES_SIZE_W, CAR_POS_FRAMES_SIZE_H,
									  CAR_POS_FRAMES_SIZE_R, WHITE);
		} else {
			view_render.drawRoundRect(CAR_POS_FRAMES_AXIS_X, frame_y,
									  CAR_POS_FRAMES_SIZE_W, CAR_POS_FRAMES_SIZE_H,
									  CAR_POS_FRAMES_SIZE_R, WHITE);
		}

		view_render.setTextColor(fg);

		if (i < CAR_POS_NUM_CARS) {
			bool on = (settingdata.num_car >> i) & 1;
			view_render.setCursor(2, frame_y + 1);
			view_render.print("Car ");
			view_render.print(i + 1);
			view_render.setCursor(104, frame_y + 1);
			view_render.print("[");
			view_render.print(on ? "1" : "0");
			view_render.print("]");
		} else {
			view_render.setCursor(45, frame_y + 1);
			view_render.print(" EXIT ");
		}
	}

	view_render.setTextColor(WHITE);
	view_render.update();
}

/*****************************************************************************/
/* Handle - Car position selection */
/*****************************************************************************/
void scr_car_position_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.clear();
		car_pos_location_chosse = CAR_POS_ITEM_ARRDESS_1;
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		switch (car_pos_location_chosse) {
		case CAR_POS_ITEM_ARRDESS_1:
		case CAR_POS_ITEM_ARRDESS_2:
		case CAR_POS_ITEM_ARRDESS_3:
		case CAR_POS_ITEM_ARRDESS_4:
		case CAR_POS_ITEM_ARRDESS_5: {
			uint8_t idx = (car_pos_location_chosse / CAR_POS_STEP_CHOSSE) - 1;
			settingdata.num_car ^= (1 << idx);
		}
			break;

		case CAR_POS_ITEM_ARRDESS_6: {
			/* Save to EEPROM and return to settings screen */
			eeprom_write(	EEPROM_SETTING_START_ADDR, \
							(uint8_t*)&settingdata, \
							sizeof(settingdata));
			SCREEN_TRAN(scr_game_setting_handle, &scr_game_setting);
			BUZZER_PlayTones(tones_startup);
		}
			break;

		default:
			break;
		}
		BUZZER_PlayTones(tones_cc);
	}
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		car_pos_location_chosse -= CAR_POS_STEP_CHOSSE;
		if (car_pos_location_chosse == CAR_POS_ITEM_ARRDESS_0) {
			car_pos_location_chosse = CAR_POS_ITEM_ARRDESS_6;
		}
		BUZZER_PlayTones(tones_cc);
	}
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		car_pos_location_chosse += CAR_POS_STEP_CHOSSE;
		if (car_pos_location_chosse > CAR_POS_ITEM_ARRDESS_6) {
			car_pos_location_chosse = CAR_POS_ITEM_ARRDESS_1;
		}
		BUZZER_PlayTones(tones_cc);
	}
		break;

	default:
		break;
	}
}

#include "scr_tombstone_count.h"
#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Tombstone count per lane */
/*****************************************************************************/
static uint8_t tb_count_location_chosse;

/*****************************************************************************/
/* View - Tombstone count per lane */
/*****************************************************************************/
static void view_scr_tombstone_count();

view_dynamic_t dyn_view_item_tombstone_count = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_tombstone_count
};

view_screen_t scr_tombstone_count = {
	&dyn_view_item_tombstone_count,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

/* Lấy số bia mộ hiện tại của lane i (0, 1, hoặc 2) */
static uint8_t get_lane_count(uint8_t i) {
	uint8_t c = 0;
	if ((settingdata.tombstone_lane_1 >> i) & 1) c++;
	if ((settingdata.tombstone_lane_2 >> i) & 1) c++;
	return c;
}

static void view_scr_tombstone_count() {
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);

	// Choice icon
	view_render.drawBitmap(	0,
							tb_count_location_chosse -
							TB_COUNT_CHOSSE_ICON_AXIS_Y,
							chosse_icon,
							TB_COUNT_CHOSSE_ICON_SIZE_W,
							TB_COUNT_CHOSSE_ICON_SIZE_H,
							WHITE);

	// 5 lane frames
	for (uint8_t i = 0; i < TB_COUNT_NUM_LANES; i++) {
		uint8_t frame_y = TB_COUNT_FRAMES_AXIS_Y_1 + TB_COUNT_FRAMES_STEP * i;

		view_render.drawRoundRect(	TB_COUNT_FRAMES_AXIS_X,
									frame_y,
									TB_COUNT_FRAMES_SIZE_W,
									TB_COUNT_FRAMES_SIZE_H,
									TB_COUNT_FRAMES_SIZE_R,
									WHITE);

		view_render.setCursor(TB_COUNT_TEXT_AXIS_X, frame_y + 1);
		view_render.print(" Lane ");
		view_render.print(i + 1);
		view_render.print("       [ ]");
		view_render.setCursor(TB_COUNT_NUMBER_AXIS_X, frame_y + 1);
		view_render.print(get_lane_count(i));
	}

	// EXIT frame
	uint8_t exit_frame_y = TB_COUNT_FRAMES_AXIS_Y_1 +
							TB_COUNT_FRAMES_STEP * TB_COUNT_NUM_LANES;
	view_render.drawRoundRect(	TB_COUNT_FRAMES_AXIS_X,
								exit_frame_y,
								TB_COUNT_FRAMES_SIZE_W,
								TB_COUNT_FRAMES_SIZE_H,
								TB_COUNT_FRAMES_SIZE_R,
								WHITE);
	view_render.setCursor(TB_COUNT_TEXT_AXIS_X + 32, exit_frame_y + 1);
	view_render.print(" EXIT ");

	view_render.update();
}

/*****************************************************************************/
/* Handle - Tombstone count per lane */
/*****************************************************************************/
void scr_tombstone_count_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.clear();
		tb_count_location_chosse = TB_COUNT_ITEM_ARRDESS_1;
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		switch (tb_count_location_chosse) {
		case TB_COUNT_ITEM_ARRDESS_1:
		case TB_COUNT_ITEM_ARRDESS_2:
		case TB_COUNT_ITEM_ARRDESS_3:
		case TB_COUNT_ITEM_ARRDESS_4:
		case TB_COUNT_ITEM_ARRDESS_5: {
			uint8_t idx = (tb_count_location_chosse / TB_COUNT_STEP_CHOSSE) - 1;
			/* Cycle: 0 → 1 → 2 → 0 */
			uint8_t cur = get_lane_count(idx);
			cur = (cur + 1) % 3;
			/* Clear cả 2 bit của lane này */
			settingdata.tombstone_lane_1 &= ~(1 << idx);
			settingdata.tombstone_lane_2 &= ~(1 << idx);
			/* Set theo giá trị mới */
			if (cur >= 1) settingdata.tombstone_lane_1 |= (1 << idx);
			if (cur == 2) settingdata.tombstone_lane_2 |= (1 << idx);
		}
			break;

		case TB_COUNT_ITEM_ARRDESS_6: {
			/* Lưu EEPROM và về settings */
			eeprom_write(	EEPROM_SETTING_START_ADDR,
							(uint8_t*)&settingdata,
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
		tb_count_location_chosse -= TB_COUNT_STEP_CHOSSE;
		if (tb_count_location_chosse == TB_COUNT_ITEM_ARRDESS_0) {
			tb_count_location_chosse = TB_COUNT_ITEM_ARRDESS_6;
		}
		BUZZER_PlayTones(tones_cc);
	}
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		tb_count_location_chosse += TB_COUNT_STEP_CHOSSE;
		if (tb_count_location_chosse > TB_COUNT_ITEM_ARRDESS_6) {
			tb_count_location_chosse = TB_COUNT_ITEM_ARRDESS_1;
		}
		BUZZER_PlayTones(tones_cc);
	}
		break;

	default:
		break;
	}
}

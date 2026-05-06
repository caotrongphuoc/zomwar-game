#include "scr_game_over.h"

/*****************************************************************************/
/* Variable Declaration - game over */
/*****************************************************************************/
static zw_game_score_t gamescore;

/*****************************************************************************/
/* View - game over */
/*****************************************************************************/
static void view_scr_game_over();

view_dynamic_t dyn_view_item_game_over = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_over
};

view_screen_t scr_game_over = {
	&dyn_view_item_game_over,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_over() {
	view_render.fillScreen(BLACK);

	view_render.drawRoundRect(29, 0, 70, 54, 20, WHITE);
	view_render.fillRect(29, 33, 21, 21, BLACK);
	view_render.drawLine(29, 33, 29, 52, WHITE);
	view_render.fillRect(78, 33, 21, 21, BLACK);
	view_render.fillRect(50, 53, 28, 1, BLACK);
	view_render.drawLine(98, 33, 98, 52, WHITE);
	view_render.drawLine(29, 52, 98, 52, WHITE);

	view_render.drawBitmap(0,   0, bitmap_spiderweb_L, 16, 16, WHITE);
	view_render.drawBitmap(110, 0, bitmap_spiderweb_R, 16, 16, WHITE);

	view_render.drawBitmap(60, 5, icon_skull, 9, 8, WHITE);

	view_render.drawLine(32, 12, 95, 15, WHITE);

	view_render.setTextSize(2);
	view_render.setTextColor(WHITE);
	view_render.setCursor(40, 16);
	view_render.print("GAME");
	view_render.setCursor(40, 31);
	view_render.print("OVER");

	view_render.setTextSize(1);
	view_render.setCursor(34, 45);
	view_render.print("Score:");
	view_render.setCursor(72, 45);
	view_render.print(gamescore.score_now);

	view_render.drawRoundRect(2, 54, 40, 10, 2, WHITE);
	view_render.setCursor(7, 55);
	view_render.print("Retry");

	view_render.drawRoundRect(44, 54, 40, 10, 2, WHITE);
	view_render.setCursor(53, 55);
	view_render.print("Rank");

	view_render.drawRoundRect(86, 54, 40, 10, 2, WHITE);
	view_render.setCursor(94, 55);
	view_render.print("Home");
}

/*****************************************************************************/
/* Handle - game over */
/*****************************************************************************/
static void rank_ranking() {
	if (gamescore.score_now > gamescore.score_1st) {
		gamescore.score_3rd = gamescore.score_2nd;
		gamescore.score_2nd = gamescore.score_1st;
		gamescore.score_1st = gamescore.score_now;
	}
	else if (gamescore.score_now > gamescore.score_2nd) {
		gamescore.score_3rd = gamescore.score_2nd;
		gamescore.score_2nd = gamescore.score_now;
	}
	else if (gamescore.score_now > gamescore.score_3rd) {
		gamescore.score_3rd = gamescore.score_now;
	}
}

void scr_game_over_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");

		view_render.initialize();
		view_render_display_on();

		eeprom_read(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));

		eeprom_read(	EEPROM_SCORE_PLAY_ADDR, \
						(uint8_t*)&gamescore.score_now, \
						sizeof(gamescore.score_now));

		rank_ranking();
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");

		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");

		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));
		SCREEN_TRAN(scr_charts_game_handle, &scr_charts_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");

		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));
		SCREEN_TRAN(scr_zw_game_handle, &scr_zomwar_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}

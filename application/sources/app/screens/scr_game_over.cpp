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

// ============ GAME OVER SCREEN ============
void view_scr_game_over() {
	view_render.fillScreen(BLACK);

	view_render.drawRoundRect(29, 0, 70, 54, 20, WHITE);
	view_render.fillRect(29, 33, 21, 21, BLACK);
	view_render.drawLine(29, 33, 29, 53, WHITE);
	view_render.fillRect(78, 33, 21, 21, BLACK);
	view_render.drawLine(98, 33, 98, 53, WHITE);
	view_render.drawLine(29, 53, 98, 53, WHITE);

  //bitmap_spiderweb
	view_render.drawBitmap(0,   0, bitmap_spiderweb_L, 16, 16, WHITE); // top-left
	view_render.drawBitmap(110, 0, bitmap_spiderweb_R, 16, 16, WHITE); // top-right

  // === Content inside tombstone ===
  // Skull icon centered at arch top  (9x8 px, x=60..68, center=64)
  view_render.drawBitmap(60, 5, icon_skull, 9, 8, WHITE);

  // Horizontal divider below skull (within arch area)
  view_render.drawLine(32, 15, 95, 15, WHITE);

  // "GAME" / "OVER" – textSize 2 (12x16 per char, 4 chars = 48px)
  //   left edge = center(64) - halfWidth(24) = 40
  view_render.setTextSize(2);
  view_render.setTextColor(WHITE);
  view_render.setCursor(40, 17);    // y=17..30 glyph (14px) + 1px padding
  view_render.print("GAME");
  view_render.setCursor(40, 32);    // y=32..45 glyph (14px) + 2px padding
  view_render.print("OVER");

  // Score – textSize 1 (6x8 per char)
  //   y=46 = OVER's blank padding row → no overlap with OVER glyph
  view_render.setTextSize(1);
  view_render.setCursor(32, 46);    // "Score:" left of center
  view_render.print("Score:");
  view_render.setCursor(72, 46);    // value right of center
  view_render.print(gamescore.score_now);

  // === Buttons (text only, 40x9 px, y=55..63) ===
  // [DOWN] = Retry game
  view_render.drawRoundRect(2, 55, 40, 9, 2, WHITE);
  view_render.setCursor(7, 56);
  view_render.print("Retry");

  // [UP] = Score board
  view_render.drawRoundRect(44, 55, 40, 9, 2, WHITE);
  view_render.setCursor(53, 56);
  view_render.print("Rank");

  // [MODE] = Home menu
  view_render.drawRoundRect(86, 55, 40, 9, 2, WHITE);
  view_render.setCursor(94, 56);
  view_render.print("Home");
}

/*****************************************************************************/
/* Handle - game over */
/*****************************************************************************/
void rank_ranking() {
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
		// View render
		view_render.initialize();
		view_render_display_on();
		// Read score 1st, 2nd, 3rd
		eeprom_read(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));
		// Read score play
		eeprom_read(	EEPROM_SCORE_PLAY_ADDR, \
						(uint8_t*)&gamescore.score_now, \
						sizeof(gamescore.score_now));
		// Reorganize
		rank_ranking();
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		// Save score and go Home menu
		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		// Save score and go Score board
		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore, \
						sizeof(gamescore));
		SCREEN_TRAN(scr_charts_game_handle, &scr_charts_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		// Save score and restart game
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

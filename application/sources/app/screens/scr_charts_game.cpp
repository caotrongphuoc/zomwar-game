#include "scr_charts_game.h"

/*****************************************************************************/
/* Variable Declaration - Charts game */
/*****************************************************************************/
static zw_game_score_t gamescore_charts;

// Spider animation
// 3 spiders hanging in the space above the tombstones
// spider_y: Y of bitmap top-left (thread starts here; body = spider_y+8..spider_y+16)
// Stagger: spider_y initial offsets create natural delayed descent
#define SPIDER_COUNT         (3)
#define SPIDER_THREAD_COL    (7)  // thread pixel column inside bitmap (col 7 = body center)
#define SPIDER_BOB_RANGE     (3)  // bob ±3 px from rest position

static const uint8_t SPIDER_X[SPIDER_COUNT]        = {28, 56, 80};
static const int8_t  SPIDER_TARGET_Y[SPIDER_COUNT] = { 4,  6,  4};

// Stagger: each spider starts 10 px above the previous (=5 ticks @2px/tick)
static const int8_t  SPIDER_INIT_Y[SPIDER_COUNT]   = {-16, -26, -36};

static int8_t  spider_y[SPIDER_COUNT];
static int8_t  spider_dir[SPIDER_COUNT];  // +1 down, -1 up
static uint8_t spider_state;              // 0=descend, 1=bob

/*****************************************************************************/
/* View - Charts game */
/*****************************************************************************/
static void view_scr_charts_game();

view_dynamic_t dyn_view_item_charts_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_charts_game
};

view_screen_t scr_charts_game = {
	&dyn_view_item_charts_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

// ============ RANKINGS SCREEN ============
void view_scr_charts_game() {
	view_render.setTextSize(1);

	// ---- Corner spider webs ----
	view_render.drawBitmap(0,   0, bitmap_spiderweb_L, 16, 16, WHITE); // top-left
	view_render.drawBitmap(110, 0, bitmap_spiderweb_R, 16, 16, WHITE); // top-right

	// ---- Spider animation ----
	for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
		uint8_t tx = SPIDER_X[i] + SPIDER_THREAD_COL;
		// Draw thread extension from screen top to bitmap top (only when below screen edge)
		if (spider_y[i] > 0) {
			view_render.drawLine(tx, 0, tx, (int16_t)spider_y[i], WHITE);
		}
		// Draw spider bitmap (8-row built-in thread + 8-row body)
		view_render.drawBitmap(SPIDER_X[i], spider_y[i], bitmap_spider, 16, 16, WHITE);
	}

	// ---- 1st place: center (x=50) ----
	view_render.setTextColor(WHITE);
	view_render.setCursor(55, 28);
	view_render.print("1st");
	view_render.drawBitmap(50, 38, bitmap_tombstone_rank, 30, 22, WHITE);
	view_render.setTextColor(BLACK);
	if(gamescore_charts.score_1st >= 0 && gamescore_charts.score_1st < 10) {
		view_render.setCursor(62, 45);
		view_render.print(gamescore_charts.score_1st);
	}
	else if(gamescore_charts.score_1st >= 10 && gamescore_charts.score_1st < 100) {
		view_render.setCursor(59, 45);
		view_render.print(gamescore_charts.score_1st);
	}
	else if(gamescore_charts.score_1st >= 100 && gamescore_charts.score_1st < 1000) {
		view_render.setCursor(56, 45);
		view_render.print(gamescore_charts.score_1st);
	}
	else {
		view_render.setCursor(53, 45);
		view_render.print(gamescore_charts.score_1st);
	}

	// ---- 2nd place: left (x=10) ----
	view_render.setTextColor(WHITE);
	view_render.setCursor(15, 30);
	view_render.print("2nd");
	view_render.drawBitmap(10, 40, bitmap_tombstone_rank, 30, 22, WHITE);
	view_render.setTextColor(BLACK);
	if(gamescore_charts.score_2nd >= 0 && gamescore_charts.score_2nd < 10)
	{
		view_render.setCursor(22, 47);
		view_render.print(gamescore_charts.score_2nd);
	}
	else if(gamescore_charts.score_2nd >= 10 && gamescore_charts.score_2nd < 100)
	{
		view_render.setCursor(19, 47);
		view_render.print(gamescore_charts.score_2nd);
	}
	else if(gamescore_charts.score_2nd >= 100 && gamescore_charts.score_2nd < 1000){
		view_render.setCursor(16, 47);
		view_render.print(gamescore_charts.score_2nd);
	}
	else {
		view_render.setCursor(13, 47);
		view_render.print(gamescore_charts.score_2nd);
	}

	// ---- 3rd place: right (x=90) ----
	view_render.setTextColor(WHITE);
	view_render.setCursor(95, 32);
	view_render.print("3rd");
	view_render.drawBitmap(90, 42, bitmap_tombstone_rank, 30, 22, WHITE);
	view_render.setTextColor(BLACK);
	if(gamescore_charts.score_3rd >= 0 && gamescore_charts.score_3rd < 10)
	{
		view_render.setCursor(102, 49);
		view_render.print(gamescore_charts.score_3rd);
	}
	else if(gamescore_charts.score_3rd >= 10 && gamescore_charts.score_3rd < 100)
	{
		view_render.setCursor(99, 49);
		view_render.print(gamescore_charts.score_3rd);
	}
	else if(gamescore_charts.score_3rd >= 100 && gamescore_charts.score_3rd < 1000) {
		view_render.setCursor(96, 49);
		view_render.print(gamescore_charts.score_3rd);
	}
	else {
		view_render.setCursor(93, 49);
		view_render.print(gamescore_charts.score_3rd);
	}
}

/*****************************************************************************/
/* Handle - Charts game */
/*****************************************************************************/
void scr_charts_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		// Read score
		eeprom_read(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore_charts, \
						sizeof(gamescore_charts));
		// Init spider animation — staggered start positions
		for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
			spider_y[i]   = SPIDER_INIT_Y[i];
			spider_dir[i] = 1;
		}
		spider_state = 0;
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_CHARTS_SPIDER_UPDATE, \
					AC_DISPLAY_CHARTS_SPIDER_UPDATE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_CHARTS_SPIDER_UPDATE: {
		if (spider_state == 0) {
			// Descend phase: all spiders move down 2px/tick until each hits target
			uint8_t all_arrived = 1;
			for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
				if (spider_y[i] < SPIDER_TARGET_Y[i]) {
					spider_y[i] += 4;
					if (spider_y[i] > SPIDER_TARGET_Y[i]) {
						spider_y[i] = SPIDER_TARGET_Y[i];
					}
					all_arrived = 0;
				}
			}
			if (all_arrived) {
				// Switch to bob — spiders 0 and 2 go down first, spider 1 goes up
				spider_dir[0] =  1;
				spider_dir[1] = -1;
				spider_dir[2] =  1;
				spider_state = 1;
			}
		} else {
			// Bob phase: gentle ±SPIDER_BOB_RANGE oscillation
			for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
				spider_y[i] += spider_dir[i];
				if (spider_y[i] >= SPIDER_TARGET_Y[i] + SPIDER_BOB_RANGE) {
					spider_dir[i] = -1;
				} else if (spider_y[i] <= SPIDER_TARGET_Y[i] - SPIDER_BOB_RANGE) {
					spider_dir[i] = 1;
				}
			}
		}
		// Render NOW (blocks during I2C bit-bang ~50-100ms on STM32L151)
		view_render_screen(&scr_charts_game);
		// Re-arm ONE_SHOT AFTER render completes — next tick fires INTERVAL ms
		// after this point, never before. Guarantees queue depth ≤ 1 at all times.
		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_CHARTS_SPIDER_UPDATE, \
					AC_DISPLAY_CHARTS_SPIDER_UPDATE_INTERVAL, \
					TIMER_ONE_SHOT);
		// Prevent scr_mng_dispatch from calling view_render_screen a second time
		SCREEN_NONE_UPDATE_MASK();
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_CHARTS_SPIDER_UPDATE);
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_CHARTS_SPIDER_UPDATE);
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_LONG_PRESSED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_LONG_PRESSED\n");
		// Reset score charts
		gamescore_charts.score_1st = 0;
		gamescore_charts.score_2nd = 0;
		gamescore_charts.score_3rd = 0;
		eeprom_write(	EEPROM_SCORE_START_ADDR, \
						(uint8_t*)&gamescore_charts, \
						sizeof(gamescore_charts));
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_CHARTS_SPIDER_UPDATE);
		SCREEN_TRAN(scr_menu_game_handle, &scr_menu_game);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}

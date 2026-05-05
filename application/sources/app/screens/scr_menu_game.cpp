#include "scr_menu_game.h"

/*****************************************************************************/
/* Variable and Struct Declaration - Menu game */
/*****************************************************************************/

#define STEP_MENU_CHOSSE				(22)
#define NUMBER_MENU_ITEMS				(4)
#define	SCREEN_MENU_H					(64)

#define MENU_ITEMS_ICON_COLOR() \
do { \
	menu_items_icon_color[0]	= !menu_chosse.items.is_item_1; \
	menu_items_icon_color[1]	= !menu_chosse.items.is_item_2; \
	menu_items_icon_color[2]	= !menu_chosse.items.is_item_3; \
	menu_items_icon_color[3]	= !menu_chosse.items.is_item_4; \
} while(0);

struct menu_items{

	unsigned int is_item_1 : 1;
	unsigned int is_item_2 : 1;
	unsigned int is_item_3 : 1;
	unsigned int is_item_4 : 1;
};

static char menu_items_name[NUMBER_MENU_ITEMS][30] = {
	"     Start ",
	"     Setting      ",
	"     Leaderboard       ",
	"     Exit         ",
};

static const uint8_t *menu_items_icon[NUMBER_MENU_ITEMS] = {
	tomb_icon,
	setting_icon,
	chart_icon,
	exit_icon,
};

uint8_t menu_items_icon_size_w[NUMBER_MENU_ITEMS] = {
	16,
	16,
	16,
	16,
};

uint8_t menu_items_icon_size_h[NUMBER_MENU_ITEMS] = {
	16,
	16,
	16,
	16,
};

uint8_t menu_items_icon_color[NUMBER_MENU_ITEMS];

uint8_t menu_items_icon_axis_y[3] = {
	2,
	24,
	46
};

typedef struct {
	int screen;
	int location;
} screen_t;

union scr_menu_t {
	uint32_t _id = 1;
	menu_items items;
};

typedef struct {
	uint8_t axis_x = 126;
	uint8_t axis_y = 0;
	uint8_t size_W = 3;
	uint8_t size_h = SCREEN_MENU_H / NUMBER_MENU_ITEMS;
} scr_menu_scroll_bar_t;

typedef struct {
	uint8_t axis_x = 0;
	uint8_t axis_y = 0;
	uint8_t size_w = 123;
	uint8_t size_h = 20;
	uint8_t size_r = 3;
} scr_menu_frames_t;

screen_t screen_menu;
scr_menu_t menu_chosse;
scr_menu_scroll_bar_t scroll_bar;
scr_menu_frames_t frame_white;
scr_menu_frames_t frame[3];

/*****************************************************************************/
/* View - Menu game */
/*****************************************************************************/
static void view_scr_menu_game();

view_dynamic_t dyn_view_menu = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu_game
};

view_screen_t scr_menu_game = {
	&dyn_view_menu,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_menu_game() {
#define ZW_GAME_MENU_ICON_AXIS_X			(7)
#define ZW_GAME_MENU_TEXT_AXIS_X			(20)

	view_render.fillRect(	scroll_bar.axis_x - 1, \
							scroll_bar.axis_y, \
							scroll_bar.size_W, \
							scroll_bar.size_h, \
							WHITE);
	view_render.drawBitmap(	scroll_bar.axis_x, \
							0, \
							dot_icon, \
							1, \
							SCREEN_MENU_H, \
							WHITE);

	view_render.fillRoundRect(	frame_white.axis_x, \
								frame_white.axis_y, \
								frame_white.size_w, \
								frame_white.size_h, \
								frame_white.size_r, \
								WHITE);
	for (uint8_t i = 0; i < 3; i++) {

		view_render.drawRoundRect(	frame[i].axis_x, \
									frame[i].axis_y, \
									frame[i].size_w, \
									frame[i].size_h, \
									frame[i].size_r, \
									WHITE);

		view_render.drawBitmap(	ZW_GAME_MENU_ICON_AXIS_X, \
								menu_items_icon_axis_y[i], \
								menu_items_icon[screen_menu.screen + i], \
								menu_items_icon_size_w[screen_menu.screen + i], \
								menu_items_icon_size_h[screen_menu.screen + i], \
								menu_items_icon_color[screen_menu.screen + i]);
	}

	view_render.setTextSize(1);
	for (uint8_t i = 0; i < 3; i++) {
		view_render.setTextColor(menu_items_icon_color[screen_menu.screen + i]);
		view_render.setCursor(ZW_GAME_MENU_TEXT_AXIS_X, menu_items_icon_axis_y[i]+5);
		view_render.print(menu_items_name[screen_menu.screen + i]);
	}
}

/*****************************************************************************/
/* Handle - Menu game */
/*****************************************************************************/
void update_menu_screen_chosse() {

	frame_white.axis_y = frame[screen_menu.location-screen_menu.screen].axis_y;
	frame[0].axis_y = 0;
	frame[1].axis_y = 22;
	frame[2].axis_y = 44;

	menu_chosse._id = 1<<screen_menu.location;
	MENU_ITEMS_ICON_COLOR();

	scroll_bar.axis_y = (SCREEN_MENU_H*screen_menu.location / NUMBER_MENU_ITEMS);
}

void screen_tran_menu() {
	switch (screen_menu.location) {
	case 0:
			SCREEN_TRAN(scr_zw_game_handle,	&scr_zomwar_game	);
		break;
	case 1:
			SCREEN_TRAN(scr_game_setting_handle,	&scr_game_setting	);
		break;
	case 2:
			SCREEN_TRAN(scr_charts_game_handle,		&scr_charts_game	);
		break;
	case 3:
			SCREEN_TRAN(scr_idle_handle,			&scr_idle			);
		break;

	default:
		break;
	}
}

void scr_menu_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		update_menu_screen_chosse();

		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		break;

	case AC_DISPLAY_SHOW_IDLE: {
		SCREEN_TRAN(scr_idle_handle,&scr_idle);
	}
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		screen_tran_menu();
	}
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		if (screen_menu.location > 0) {
			screen_menu.location--;
			if (frame_white.axis_y == frame[0].axis_y) {
				if (screen_menu.screen > 0) {
					screen_menu.screen--;
				}
			}
			else if (frame_white.axis_y == frame[1].axis_y) {
				frame_white.axis_y = frame[0].axis_y;
			}
			else if (frame_white.axis_y == frame[2].axis_y) {
				frame_white.axis_y = frame[1].axis_y;
			}
		} else {
			screen_menu.location = NUMBER_MENU_ITEMS - 1;
			screen_menu.screen   = NUMBER_MENU_ITEMS - 3;
		}
		update_menu_screen_chosse();

		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: {
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		if (screen_menu.location < NUMBER_MENU_ITEMS - 1) {
			screen_menu.location++;
			if (frame_white.axis_y == frame[0].axis_y) {
				frame_white.axis_y = frame[1].axis_y;
			}
			else if (frame_white.axis_y == frame[1].axis_y) {
				frame_white.axis_y = frame[2].axis_y;
			}
			else if (frame_white.axis_y == frame[2].axis_y) {
				if (screen_menu.screen < NUMBER_MENU_ITEMS - 3) {
					screen_menu.screen++;
				}
			}
		} else {
			screen_menu.location = 0;
			screen_menu.screen   = 0;
		}
		update_menu_screen_chosse();

		timer_set(	AC_TASK_DISPLAY_ID, \
					AC_DISPLAY_SHOW_IDLE, \
					AC_DISPLAY_IDLE_INTERVAL, \
					TIMER_ONE_SHOT);
	}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}

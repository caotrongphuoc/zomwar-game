#include "scr_zomwar_game.h"
#include "screens.h"

/*****************************************************************************/
/* Variable Declaration - Zomwar game screen */
/*****************************************************************************/
uint8_t ar_game_state;
ar_game_setting_t settingsetup;

#define NUM_BATS 2

static struct {
	int16_t x;
	uint8_t y;
	int8_t  dir;        // +1 = left→right, -1 = right→left
	uint8_t frame;      // 1, 2, 3
	uint8_t frame_tick;
} bat_anim[NUM_BATS];

/*****************************************************************************/
/* View - Zomwar game screen*/
/*****************************************************************************/
void ar_game_frame_display() {
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(2,55);
	view_render.print("Kills:");
	view_render.print(ar_game_kill_count);
	view_render.setCursor(54,55);
	view_render.print(" Scores:");
	view_render.print(ar_game_score);
	view_render.drawLine(0, LCD_HEIGHT, 	LCD_WIDTH, LCD_HEIGHT,		WHITE);
	view_render.drawLine(0, LCD_HEIGHT-10, 	LCD_WIDTH, LCD_HEIGHT-10,	WHITE);
	view_render.drawRect(0, 0, 128, 64, 1);
}

void ar_game_zomwar_display() {
    if (zomwar.visible == WHITE) {
        const unsigned char* frame = (zomwar.action_image == 2) ? bitmap_peashooter_II : bitmap_peashooter_I;
        view_render.drawBitmap( zomwar.x, \
                                zomwar.y - 10, \
                                frame, \
                                SIZE_BITMAP_ZOMWAR_X, \
                                SIZE_BITMAP_ZOMWAR_Y, \
                                WHITE);
    }
}

void ar_game_bullet_display() {
	for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) {
		if (bullet[i].visible == WHITE) {
			view_render.drawBitmap(	bullet[i].x, \
									bullet[i].y, \
									bitmap_bullet, \
									SIZE_BITMAP_BULLET_X, \
									SIZE_BITMAP_BULLET_Y, \
									WHITE);
		}
	}
}

void ar_game_zombie_display() {
	for (uint8_t i = 0; i < NUM_ZOMBIES; i++) {
		if (zombie[i].visible == WHITE) {
			if (zombie[i].action_image == 1) {
				view_render.drawBitmap(	zombie[i].x, \
										zombie[i].y, \
										bitmap_zombie_I, \
										SIZE_BITMAP_ZOMBIES_X, \
										SIZE_BITMAP_ZOMBIES_Y, \
				 						WHITE);
			}
			else if (zombie[i].action_image == 2) {
				view_render.drawBitmap(	zombie[i].x, \
										zombie[i].y, \
										bitmap_zombie_II, \
										SIZE_BITMAP_ZOMBIES_X, \
										SIZE_BITMAP_ZOMBIES_Y, \
										WHITE);
			}
			else if (zombie[i].action_image == 3) {
				view_render.drawBitmap(	zombie[i].x, \
										zombie[i].y, \
										bitmap_zombie_III, \
										SIZE_BITMAP_ZOMBIES_X, \
										SIZE_BITMAP_ZOMBIES_Y, \
				 						WHITE);
			}
		}
	}
}

void ar_game_bang_display() {
	for (uint8_t i = 0; i < NUM_BANG; i++) {
		if (bang[i].visible == WHITE) {
			if (bang[i].action_image == 1) {
				view_render.drawBitmap(	bang[i].x, \
										bang[i].y, \
										bitmap_bang_I, \
										SIZE_BITMAP_BANG_I_X, \
										SIZE_BITMAP_BANG_I_Y, \
										WHITE);
			}
			else if (bang[i].action_image == 2) {
				view_render.drawBitmap(	bang[i].x, \
										bang[i].y, \
										bitmap_bang_II, \
										SIZE_BITMAP_BANG_I_X, \
										SIZE_BITMAP_BANG_I_Y, \
				 						WHITE);
			}
			else if (bang[i].action_image == 3) {
				view_render.drawBitmap( bang[i].x + 2, \
										bang[i].y - 1, \
										bitmap_bang_III, \
										SIZE_BITMAP_BANG_II_X, \
										SIZE_BITMAP_BANG_II_Y, \
				 						WHITE);
			}
		}
	}
}

void ar_game_car_display() {
    for (uint8_t i = 0; i < NUM_LANES; i++) {

        // Kiểm tra xem xe ở làn này có đang hiển thị không
        if (car[i].visible) { // Hoặc car[i].visible == WHITE tùy hệ thống của bạn

            // Kiểm tra trạng thái khung hình để vẽ đúng Bitmap
            if (car[i].action_image == 1) {
                view_render.drawBitmap( car[i].x, \
                                        car[i].y, \
                                        bitmap_car_I, \
                                        SIZE_BITMAP_CAR_X, \
                                        SIZE_BITMAP_CAR_Y, \
                                        WHITE);
            }
            else if (car[i].action_image == 2) {
                view_render.drawBitmap( car[i].x, \
                                        car[i].y, \
                                        bitmap_car_II, \
                                        SIZE_BITMAP_CAR_X, \
                                        SIZE_BITMAP_CAR_Y, \
                                        WHITE);
            }
            else if (car[i].action_image == 3) {
                view_render.drawBitmap( car[i].x, \
                                        car[i].y, \
                                        bitmap_car_III, \
                                        SIZE_BITMAP_CAR_X, \
                                        SIZE_BITMAP_CAR_Y, \
                                        WHITE);
            }
        }
    }
}

void ar_game_grass_display() {
    static const uint8_t ly[NUM_LANES] = LANE_Y;
    for (uint8_t l = 0; l < NUM_LANES; l++) {
        uint8_t gy = ly[l] + 9;  // hàng cuối của lane
        for (uint8_t x = 0; x < 128; x += 5) {
            view_render.drawPixel(x,     gy,     WHITE); // Gốc
			view_render.drawPixel(x + 1, gy,     WHITE); // Thân dưới
			view_render.drawPixel(x + 2, gy,     WHITE); // Thân dưới
			view_render.drawPixel(x + 2, gy - 1, WHITE); // Thân trên vươn ngang
			view_render.drawPixel(x + 3, gy - 1, WHITE); // Ngọn cỏ nằm ngang
        }
    }
}

void ar_game_tombstone_display() {
    static const uint8_t ly[NUM_LANES] = LANE_Y;
    for (uint8_t i = 0; i < NUM_TOMBSTONES; i++) {
        if (!tombstones[i].active) continue;
        view_render.drawBitmap(
            tombstones[i].x,
            ly[tombstones[i].lane],
            bitmap_tombstone,
            SIZE_BITMAP_TOMBSTONE_X,
            SIZE_BITMAP_TOMBSTONE_Y,
            WHITE);
    }
}

void ar_game_level_display() {
    if (level_up_display_timer == 0) return;
    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    /* "LVN": mỗi ký tự size-2 rộng 12px, cao 16px */
    uint8_t x = (wave_level < 10) ? (128 - 36) / 2   /* "LV0" = 36px */
                                   : (128 - 48) / 2;  /* "LV10" = 48px */
    view_render.setCursor(x, (54 - 16) / 2);
    view_render.print("LV");
    view_render.print(wave_level);
    view_render.setTextSize(1);
}

static void ar_game_bat_init() {
	bat_anim[0].dir        = (ar_game_score & 1) ? 1 : -1;
	bat_anim[0].y          = 4 + (uint8_t)(ar_game_score % 18);
	bat_anim[0].x          = (bat_anim[0].dir == 1) ? -16 : 128;
	bat_anim[0].frame      = 1;
	bat_anim[0].frame_tick = 0;

	bat_anim[1].dir        = -bat_anim[0].dir;
	bat_anim[1].y          = bat_anim[0].y + 20;
	bat_anim[1].x          = (bat_anim[1].dir == 1) ? -16 : 128;
	bat_anim[1].frame      = 2;
	bat_anim[1].frame_tick = 1;
}

static void ar_game_bat_update() {
	for (uint8_t i = 0; i < NUM_BATS; i++) {
		bat_anim[i].x += bat_anim[i].dir * 2;
		if (++bat_anim[i].frame_tick >= 3) {
			bat_anim[i].frame_tick = 0;
			bat_anim[i].frame      = (bat_anim[i].frame % 3) + 1;
		}
		if (bat_anim[i].dir == 1 && bat_anim[i].x >= 128) {
			bat_anim[i].x = -16;
		} else if (bat_anim[i].dir == -1 && bat_anim[i].x <= -16) {
			bat_anim[i].x = 128;
		}
	}
}

static void ar_game_bat_display() {
	const unsigned char* frames[3] = {bitmap_bat_I, bitmap_bat_II, bitmap_bat_III};
	for (uint8_t i = 0; i < NUM_BATS; i++) {
		view_render.drawBitmap(bat_anim[i].x, bat_anim[i].y,
		                       frames[bat_anim[i].frame - 1],
		                       16, 16, WHITE);
	}
}

void ar_game_warning_display() {
    if (wave_warning_active) {
        // Chớp tắt: hiện mỗi WARNING_BLINK_RATE tick
        if ((wave_warning_timer / WARNING_BLINK_RATE) % 2 == 0) {
            // Vẽ icon warning giữa màn hình
            view_render.drawBitmap(
                (LCD_WIDTH - SIZE_BITMAP_WARNING_X) / 2,   // X giữa
                (54 - SIZE_BITMAP_WARNING_Y) / 2,          // Y giữa vùng game (trên thanh score)
                bitmap_warning,
                SIZE_BITMAP_WARNING_X,
                SIZE_BITMAP_WARNING_Y,
                WHITE);
        }
    }
}

static void view_scr_zomwar_game();

view_dynamic_t dyn_view_item_zomwar_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_zomwar_game
};

view_screen_t scr_zomwar_game = {
	&dyn_view_item_zomwar_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_zomwar_game() {
	if (ar_game_state == GAME_PLAY) {
		ar_game_frame_display();
		ar_game_grass_display();
		ar_game_tombstone_display();
		ar_game_zomwar_display();
		ar_game_bullet_display();
		ar_game_zombie_display();
		ar_game_bang_display();
		ar_game_car_display();
		ar_game_warning_display();
	}
	else if (ar_game_state == GAME_OVER) {
		view_render.clear();
		view_render.drawBitmap(15, 4, bitmap_rip, 100, 54, WHITE);
		view_render.drawBitmap(0, 48, bitmap_spiderweb_BL, 16, 16, WHITE); // bottom-left
		view_render.drawBitmap(110, 48, bitmap_spiderweb_BR, 16, 16, WHITE); // bottom-right
		view_render.drawBitmap(85, 8, bitmap_branch_R, 50, 17, WHITE);
		view_render.drawBitmap(-2, 4, bitmap_branch_L, 50, 17, WHITE);
		
		ar_game_bat_display();
	}
}

/*****************************************************************************/
/* Handle - Zomwar game screen */
/*****************************************************************************/
void ar_game_level_setup() {
	eeprom_read(	EEPROM_SETTING_START_ADDR, \
					(uint8_t*)&settingsetup, \
					sizeof(settingsetup));
}

void ar_game_time_tick_setup() {
	timer_set(	AC_TASK_DISPLAY_ID, \
				AR_GAME_TIME_TICK, \
				AR_GAME_TIME_TICK_INTERVAL, \
				TIMER_PERIODIC);
}

void ar_game_save_and_reset_score() {
	eeprom_write(	EEPROM_SCORE_PLAY_ADDR, \
					(uint8_t*)&ar_game_score, \
					sizeof(ar_game_score));
}

void scr_zomwar_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: {
		APP_DBG_SIG("SCREEN_ENTRY\n");
		// Level setup
		ar_game_level_setup();
		
		// Setup game Object
		task_post_pure_msg(AR_GAME_ZOMWAR_ID, 	 	AR_GAME_ZOMWAR_SETUP);
		task_post_pure_msg(AR_GAME_BULLET_ID, 	 	AR_GAME_BULLET_SETUP);
		task_post_pure_msg(AR_GAME_ZOMBIE_ID, 		AR_GAME_ZOMBIE_SETUP);
		task_post_pure_msg(AR_GAME_BANG_ID, 	 	AR_GAME_BANG_SETUP);
		task_post_pure_msg(AR_GAME_BORDER_ID, 	 	AR_GAME_BORDER_SETUP);
		task_post_pure_msg(AR_GAME_CAR_ID, 			AR_GAME_CAR_SETUP);
		// Setup timer
		ar_game_time_tick_setup();
		// State update
		ar_game_state = GAME_PLAY;
	}
		break;

	case AR_GAME_TIME_TICK: {
		APP_DBG_SIG("AR_GAME_TIME_TICK\n");
		if (ar_game_state == GAME_PLAY) {
			task_post_pure_msg(AR_GAME_ZOMWAR_ID, 		AR_GAME_ZOMWAR_UPDATE);
			task_post_pure_msg(AR_GAME_BULLET_ID, 		AR_GAME_BULLET_RUN);
			task_post_pure_msg(AR_GAME_ZOMBIE_ID, 		AR_GAME_ZOMBIE_DETONATOR);
			task_post_pure_msg(AR_GAME_ZOMBIE_ID, 		AR_GAME_ZOMBIE_RUN);
			task_post_pure_msg(AR_GAME_BANG_ID, 		AR_GAME_BANG_UPDATE);
			task_post_pure_msg(AR_GAME_BORDER_ID, 		AR_GAME_LEVEL_UP);
			task_post_pure_msg(AR_GAME_CAR_ID,  		AR_GAME_CAR_RUN);
			task_post_pure_msg(AR_GAME_BORDER_ID, 		AR_GAME_CHECK_GAME_OVER);
		}
		else if (ar_game_state == GAME_OVER) {
			ar_game_bat_update();
		}
	}
		break;

	case AR_GAME_RESET: {
		APP_DBG_SIG("AR_GAME_RESET\n");
		// Reset game Object
		task_post_pure_msg(AR_GAME_ZOMWAR_ID, 		AR_GAME_ZOMWAR_RESET);
		task_post_pure_msg(AR_GAME_BULLET_ID, 		AR_GAME_BULLET_RESET);
		task_post_pure_msg(AR_GAME_ZOMBIE_ID,		AR_GAME_ZOMBIE_RESET);
		task_post_pure_msg(AR_GAME_BANG_ID, 		AR_GAME_BANG_RESET);
		task_post_pure_msg(AR_GAME_BORDER_ID, 		AR_GAME_BORDER_RESET);
		task_post_pure_msg(AR_GAME_CAR_ID,  		AR_GAME_CAR_RESET);
		// Timer Exit
		timer_set(	AC_TASK_DISPLAY_ID, \
					AR_GAME_EXIT_GAME, \
					AR_GAME_TIME_EXIT_INTERVAL, \
					TIMER_ONE_SHOT);
		// Save and reset Score
		ar_game_save_and_reset_score();
		// Init bat flying animation
		ar_game_bat_init();
		// State update
		ar_game_state = GAME_OVER;
	}
		BUZZER_PlayTones(tones_3beep);
		break;

	case AR_GAME_EXIT_GAME: {
		APP_DBG_SIG("AR_GAME_EXIT_GAME\n");
		// State update
		ar_game_state = GAME_OFF;
		// Change the screen
		SCREEN_TRAN(scr_game_over_handle, &scr_game_over);		
	}
		break;

	default:
		break;
	}
}

#pragma once
#include "inttypes.h"
#include <pico/stdlib.h>
#include "../lib/bsp/bsp_cst328.h"
#include "../lib/joysticks/Joystics.h"

#define TOUCH_COUNT (5)

#define KEYS_COUNT (5)
#define SLIDERS_COUNT (1)
#define SLIDERS_SENSE (4)

#define SINGLE_TAP_TICKS (500)

#define DOUBLE_TAP (10*1000)
#define DOUBLE_TAP_CNT (5)
#define DOUBLE_TAP_PRESS (40)

#define SOFTKEY_TIME	0xF100
#define SOFTKEY_GAME_A	0xF200
#define SOFTKEY_GAME_B	0xF300
#define SOFTKEY_ALARM	0xF400
#define SOFTKEY_RESET	0xF500

enum tsDirection_e { TSD_STAY=0, TSD_LEFT, TSD_RIGHT, TSD_UP, TSD_DOWN, TDS_REQUEST };

uint16_t decode_touch_game(bsp_cst328_data_t cst328_data);
uint16_t decode_touch_main(bsp_cst328_data_t cst328_data);
uint16_t decode_touch_menu(bsp_cst328_data_t cst328_data);
uint16_t decode_touch_all(bsp_cst328_data_t cst328_data, bool is_touch);
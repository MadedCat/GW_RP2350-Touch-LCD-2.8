#pragma once
#include "inttypes.h"
#include <pico/stdlib.h>

#define FAST_MENU_MAIN 0

#define FAST_SUBMENU_LINES 10


#define FAST_MENU_COUNT 1
#define FAST_MENU_LINES 12


#define FAST_MAIN_MANAGER		(0)
#define FAST_GAME_A				(1)
#define FAST_GAME_B				(2)
#define FAST_TIME				(3)
#define FAST_ALARM				(4)
#define FAST_SET_CLOCK			(5)
#define FAST_MAIN_HELP			(6)
#define FAST_MAIN_SETTINGS		(7)
#define FAST_MAIN_SOFT_RESET	(8)
#define FAST_MAIN_HARD_RESET	(9)


const uint8_t __in_flash() *fast_menu_lines[FAST_MENU_COUNT]={(uint8_t*)9}; //,(uint8_t*)5,(uint8_t*)11,(uint8_t*)6,(uint8_t*)9
const char __in_flash() *fast_menu[FAST_MENU_COUNT][FAST_MENU_LINES]={
	{ 
		" [FILE BROWSER] ", //0
		"     Game A     ", //1
		"     Game B     ", //2
		"      TIME      ", //3
		"     ALARM      ", //4
		"   [SET CLOCK]  ", //5
		"     [HELP]     ", //6
		"   [SETTINGS]   ", //7
		"   Soft reset   ", //8
		"   Hard reset   ", //9
		"\0", //10
	}
};
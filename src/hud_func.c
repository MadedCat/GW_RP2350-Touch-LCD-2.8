#include <pico.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include "hud_func.h"
#include "globals.h"


uint32_t hud_timer;
void* hud_ptr;
uint16_t old_hud_mode=0;
uint16_t current_hud_mode=0;
char hud_text[104];

uint8_t battery_status=0;
uint8_t battery_status_ico=0;

uint8_t vol_strength	= 0;
uint8_t vol_bank 		= 0;

/*
#define CL_BLACK	 0x00
#define CL_BLUE	  0x01
#define CL_RED	   0x02
#define CL_PINK	  0x03
#define CL_GREEN	 0x04
#define CL_CYAN	  0x05
#define CL_YELLOW	0x06
#define CL_GRAY	  0x07
#define CL_LT_BLACK  0x08
#define CL_LT_BLUE   0x09
#define CL_LT_RED	0x0a
#define CL_LT_PINK   0x0b
#define CL_LT_GREEN  0x0c
#define CL_LT_CYAN   0x0d
#define CL_LT_YELLOW 0x0e
#define CL_WHITE	 0x0f
*/ 

/*-------Graphics--------*/
/*typedef struct HUD_struct {
	bool active;
	bool changed;
	int16_t  first_line;
	int16_t  last_line;
	int16_t  left_pos;
	int16_t	 img_width;
	uint8_t* img_ptr;
	int16_t	 ptr_inc;
	int16_t	 ptr_addr;
} __attribute__((packed)) HUD_data;

HUD_data hud[10];
*/

short int img_up=0;
short int img_down=0;
short int img_left=0;
short int img_right=0;
short int img_first_line=0;
short int img_last_line=0;


#pragma GCC push_options
#if COMPOSITE_TV||SOFT_COMPOSITE_TV
#pragma GCC optimize("-Ofast")
#else
#pragma GCC optimize("-Os")
#endif
bool FAST_FUNC(hud_battery)(short int line){
	if(line==(ICON_BAT_TOP-1)){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_BAT_BOT+1)){memset(hud_line,0x10,SCREEN_W);return false;}
	//memset(hud_line,0x10,SCREEN_W);	//fill buffer EMPTY color
	uint16_t addr=0;

	if((line>=ICON_BAT_TOP)&&(line<=ICON_BAT_BOT)){
		//draw_bufline_text5x7_len(&hud_line[16],(line-ICON_BAT_TOP),&hud_text[0],CL_LT_YELLOW-PALETTE_SHIFT,CL_EMPTY,12);
		draw_bufline_text_len(&hud_line[8],(line-ICON_BAT_TOP),&hud_text[0],CL_LT_YELLOW-PALETTE_SHIFT,CL_EMPTY,22);
		if((battery_status&0xF0)>0){
			img_up			= (ico_yfpos[ico_battery_map[10+(battery_status>>4)]]);
			img_down		= (ico_ytpos[ico_battery_map[10+(battery_status>>4)]]);
			img_left		= (ico_xfpos[ico_battery_map[10+(battery_status>>4)]]);
			img_right		= (ico_xtpos[ico_battery_map[10+(battery_status>>4)]]);
			img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_CHG_LEFT],&icons[addr+img_left],(img_right-img_left));
			}
		};
		img_up			= (ico_yfpos[ico_battery_map[battery_status&0x0f]]);
		img_down		= (ico_ytpos[ico_battery_map[battery_status&0x0f]]);
		img_left		= (ico_xfpos[ico_battery_map[battery_status&0x0f]]);
		img_right		= (ico_xtpos[ico_battery_map[battery_status&0x0f]]);
		img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
		if(line<img_last_line){
			addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
			memcpy(&hud_line[ICON_BAT_LEFT],&icons[addr+img_left],(img_right-img_left));
			//memcpy(&hud_line[48],&icons[addr+img_left],(img_right-img_left));
		}
		//memset(&hud_line[0],CL_RED-PALETTE_SHIFT,12*FONT_5x7_W);
		return true;
	}
	return false;
}
#pragma GCC pop_options



#pragma GCC push_options
#if COMPOSITE_TV||SOFT_COMPOSITE_TV
#pragma GCC optimize("-Ofast")
#else
#pragma GCC optimize("-Os")
#endif
bool FAST_FUNC(hud_scale)(short int line){
	//if(!show_hud) return false;
	if(line<1){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_BAT_BOT+1)){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_VOL_TOP-1)){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_VOL_BOT+1)){memset(hud_line,0x10,SCREEN_W);return false;}
	uint16_t addr=0;
	if(current_hud_mode&HM_SHOW_BATTERY){
		if((line>=ICON_BAT_TOP)&&(line<=ICON_BAT_BOT)){
			if((battery_status&0xF0)>0){
				img_up			= (ico_yfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_down		= (ico_ytpos[ico_battery_map[10+(battery_status>>4)]]);
				img_left		= (ico_xfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_right		= (ico_xtpos[ico_battery_map[10+(battery_status>>4)]]);
				img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
				if(line<img_last_line){
					addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_CHG_LEFT],&icons[addr+img_left],(img_right-img_left));
				}
			};
			img_up			= (ico_yfpos[ico_battery_map[battery_status&0x0f]]);
			img_down		= (ico_ytpos[ico_battery_map[battery_status&0x0f]]);
			img_left		= (ico_xfpos[ico_battery_map[battery_status&0x0f]]);
			img_right		= (ico_xtpos[ico_battery_map[battery_status&0x0f]]);
			img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_BAT_LEFT],&icons[addr+img_left],(img_right-img_left));
				//return true;
			}
		}
	}
	if(current_hud_mode&HM_SHOW_VOLUME){
		//memset(hud_line,0x10,SCREEN_W);
		if((line>=ICON_VOL_TOP)&&(line<=ICON_VOL_BOT)){
			/*if(line>=ICON_VOL_STRENGTH_TOP){				
				img_up			= (ico_yfpos[ico_vol_map[vol_strength]]);
				img_down		= (ico_ytpos[ico_vol_map[vol_strength]]);
				img_left		= (ico_xfpos[ico_vol_map[vol_strength]]);
				img_right		= (ico_xtpos[ico_vol_map[vol_strength]]);
				img_last_line	= (ICON_VOL_STRENGTH_TOP+(img_down-img_up));
				if(line<img_last_line){
					addr=(((line-ICON_VOL_STRENGTH_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_VOL_STRENGTH_LEFT],&icons[addr+img_left],(img_right-img_left));
				}	
			}*/
			if(line>=ICON_VOL_NAME_TOP){
				img_up			= (ico_yfpos[ico_vol_scale_map[0]]);
				img_down		= (ico_ytpos[ico_vol_scale_map[0]]);
				img_left		= (ico_xfpos[ico_vol_scale_map[0]]);
				img_right		= (ico_xtpos[ico_vol_scale_map[0]]);
				img_last_line	= (ICON_VOL_NAME_TOP+(img_down-img_up));
				if(line<img_last_line){
					addr=(((line-ICON_VOL_NAME_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_VOL_NAME_LEFT],&icons[addr+img_left],(img_right-img_left));
					return true;
				}	
			}
			if(line>=ICON_VOL_SCALE_TOP){
				img_up			= (ico_yfpos[ico_vol_scale_map[1]]);
				img_down		= (ico_ytpos[ico_vol_scale_map[1]]);
				img_left		= (ico_xfpos[ico_vol_scale_map[1]]);
				img_right		= (ico_xtpos[ico_vol_scale_map[1]]);
				img_last_line	= (ICON_VOL_SCALE_TOP+(img_down-img_up));
				img_first_line	= (ico_yfpos[ico_vol_scale_map[2]]);

				if(line<=img_last_line){
					addr=(((line-ICON_VOL_SCALE_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_VOL_SCALE_LEFT],&icons[addr+img_left],(img_right-img_left));
					addr=(((line-ICON_VOL_SCALE_TOP)+img_first_line)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_VOL_SCALE_LEFT],&icons[addr+img_left],(5*(cfg_volume/8))+1);
					return true;
				}	
			}		
		}
	}
	if(current_hud_mode&HM_SHOW_BRIGHT){
		if((line>=ICON_BRI_TOP)&&(line<=ICON_BRI_BOT)){
			if(line>=ICON_BRI_NAME_TOP){
				img_up			= (ico_yfpos[ico_bri_scale_map[0]]);
				img_down		= (ico_ytpos[ico_bri_scale_map[0]]);
				img_left		= (ico_xfpos[ico_bri_scale_map[0]]);
				img_right		= (ico_xtpos[ico_bri_scale_map[0]]);
				img_last_line	= (ICON_BRI_NAME_TOP+(img_down-img_up));
				if(line<img_last_line){
					addr=(((line-ICON_BRI_NAME_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_BRI_NAME_LEFT],&icons[addr+img_left],(img_right-img_left));
					return true;
				}	
			}
			if(line>=ICON_BRI_SCALE_TOP){
				img_up			= (ico_yfpos[ico_bri_scale_map[1]]);
				img_down		= (ico_ytpos[ico_bri_scale_map[1]]);
				img_left		= (ico_xfpos[ico_bri_scale_map[1]]);
				img_right		= (ico_xtpos[ico_bri_scale_map[1]]);
				img_last_line	= (ICON_BRI_SCALE_TOP+(img_down-img_up));
				img_first_line	= (ico_yfpos[ico_bri_scale_map[2]]);
				if(line<=img_last_line){
					addr=(((line-ICON_BRI_SCALE_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_BRI_SCALE_LEFT],&icons[addr+img_left],(img_right-img_left));
					addr=(((line-ICON_BRI_SCALE_TOP)+img_first_line)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_BRI_SCALE_LEFT],&icons[addr+img_left],(16*(cfg_brightness))+1);
					return true;
				}	
			}		
		}		
	}

	return false;
}
#pragma GCC pop_options

/*
bool hud_main_kb(short int line);
bool hud_game_kb(short int line);
bool hud_menu_kb(short int line);
*/

#pragma GCC push_options
#if COMPOSITE_TV||SOFT_COMPOSITE_TV
#pragma GCC optimize("-Ofast")
#else
#pragma GCC optimize("-Os")
#endif
bool FAST_FUNC(hud_main_kb)(short int line){
	if(line<1){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_BAT_BOT+1)){memset(&hud_line[(SCREEN_W/2)],0x10,(SCREEN_W/2));/*return false;*/}
	//if(line==(ICON_MENU_BOTTOM)){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_MAIN_BTNS_TOP-1)){memset(hud_line,0x10,SCREEN_W);return false;}

	uint16_t addr=0;
	if(current_hud_mode&HM_SHOW_BATTERY){
		if((line>=ICON_BAT_TOP)&&(line<=ICON_BAT_BOT)){
			draw_bufline_text_len(&hud_line[86],(line-ICON_BAT_TOP),&hud_text[0],CL_LT_YELLOW-PALETTE_SHIFT,CL_EMPTY,22);
			if((battery_status&0xF0)>0){
				img_up			= (ico_yfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_down		= (ico_ytpos[ico_battery_map[10+(battery_status>>4)]]);
				img_left		= (ico_xfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_right		= (ico_xtpos[ico_battery_map[10+(battery_status>>4)]]);
				img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
				if(line<img_last_line){
					addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_CHG_LEFT],&icons[addr+img_left],(img_right-img_left));
				}
			};
			img_up			= (ico_yfpos[ico_battery_map[battery_status&0x0f]]);
			img_down		= (ico_ytpos[ico_battery_map[battery_status&0x0f]]);
			img_left		= (ico_xfpos[ico_battery_map[battery_status&0x0f]]);
			img_right		= (ico_xtpos[ico_battery_map[battery_status&0x0f]]);
			img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_BAT_LEFT],&icons[addr+img_left],(img_right-img_left));
				//return true;
			}
		}
	}
	if((current_hud_mode&HM_SHOW_MAINBTN)){
		if(line>=ICON_MENU_TOP){
			img_up			= (ico_yfpos[ico_main_screen_map[0]]);
			img_down		= (ico_ytpos[ico_main_screen_map[0]]);
			img_left		= (ico_xfpos[ico_main_screen_map[0]]);
			img_right		= (ico_xtpos[ico_main_screen_map[0]]);
			img_last_line	= (ICON_MENU_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_LEFT],&icons[addr+img_left],(img_right-img_left));
				return true;
			}
		}
		if(line>=ICON_MAIN_BTNS_TOP){
			img_up			= (ico_yfpos[ico_main_screen_map[1]]);
			img_down		= (ico_ytpos[ico_main_screen_map[1]]);
			img_left		= (ico_xfpos[ico_main_screen_map[1]]);
			img_right		= (ico_xtpos[ico_main_screen_map[1]]);
			img_last_line	= (ICON_MAIN_BTNS_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MAIN_BTNS_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MAIN_BTNS_LEFT1],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_main_screen_map[2]]);
			img_down		= (ico_ytpos[ico_main_screen_map[2]]);
			img_left		= (ico_xfpos[ico_main_screen_map[2]]);
			img_right		= (ico_xtpos[ico_main_screen_map[2]]);
			img_last_line	= (ICON_MAIN_BTNS_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MAIN_BTNS_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MAIN_BTNS_LEFT2],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_main_screen_map[3]]);
			img_down		= (ico_ytpos[ico_main_screen_map[3]]);
			img_left		= (ico_xfpos[ico_main_screen_map[3]]);
			img_right		= (ico_xtpos[ico_main_screen_map[3]]);
			img_last_line	= (ICON_MAIN_BTNS_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MAIN_BTNS_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MAIN_BTNS_LEFT3],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_main_screen_map[4]]);
			img_down		= (ico_ytpos[ico_main_screen_map[4]]);
			img_left		= (ico_xfpos[ico_main_screen_map[4]]);
			img_right		= (ico_xtpos[ico_main_screen_map[4]]);
			img_last_line	= (ICON_MAIN_BTNS_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MAIN_BTNS_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MAIN_BTNS_LEFT4],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}

	}
	return false;
}
#pragma GCC pop_options

#pragma GCC push_options
#if COMPOSITE_TV||SOFT_COMPOSITE_TV
#pragma GCC optimize("-Ofast")
#else
#pragma GCC optimize("-Os")
#endif
bool FAST_FUNC(hud_game_kb)(short int line){

	if(line<1){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_BAT_BOT+1)){memset(&hud_line[(SCREEN_W/2)],0x10,(SCREEN_W/2));/*return false;*/}
	//if(line==(ICON_MENU_BOTTOM)){memset(hud_line,0x10,SCREEN_W);return false;}
	//if(line==(ICON_MAIN_BTNS_TOP-1)){memset(hud_line,0x10,SCREEN_W);return false;}

	uint16_t addr=0;
	if(current_hud_mode&HM_SHOW_BATTERY){
		if((line>=ICON_BAT_TOP)&&(line<=ICON_BAT_BOT)){
			draw_bufline_text_len(&hud_line[86],(line-ICON_BAT_TOP),&hud_text[0],CL_LT_YELLOW-PALETTE_SHIFT,CL_EMPTY,22);
			if((battery_status&0xF0)>0){
				img_up			= (ico_yfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_down		= (ico_ytpos[ico_battery_map[10+(battery_status>>4)]]);
				img_left		= (ico_xfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_right		= (ico_xtpos[ico_battery_map[10+(battery_status>>4)]]);
				img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
				if(line<img_last_line){
					addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_CHG_LEFT],&icons[addr+img_left],(img_right-img_left));
				}
			};
			img_up			= (ico_yfpos[ico_battery_map[battery_status&0x0f]]);
			img_down		= (ico_ytpos[ico_battery_map[battery_status&0x0f]]);
			img_left		= (ico_xfpos[ico_battery_map[battery_status&0x0f]]);
			img_right		= (ico_xtpos[ico_battery_map[battery_status&0x0f]]);
			img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_BAT_LEFT],&icons[addr+img_left],(img_right-img_left));
				//return true;
			}
		}
	}	
	if((current_hud_mode&HM_SHOW_GAMEBTN)){
		if((line>=ICON_MENU_TOP)&&(line<=ICON_GAME_BTNS_TOP2)){
			img_up			= (ico_yfpos[ico_game_screen_map[0]]);
			img_down		= (ico_ytpos[ico_game_screen_map[0]]);
			img_left		= (ico_xfpos[ico_game_screen_map[0]]);
			img_right		= (ico_xtpos[ico_game_screen_map[0]]);
			img_last_line	= (ICON_MENU_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_LEFT],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}
		if((line>=ICON_GAME_BTNS_TOP2)&&(line<=ICON_GAME_BTNS_BOT)){
			img_up			= (ico_yfpos[ico_game_screen_map[5]]);
			img_down		= (ico_ytpos[ico_game_screen_map[5]]);
			img_left		= (ico_xfpos[ico_game_screen_map[5]]);
			img_right		= (ico_xtpos[ico_game_screen_map[5]]);
			img_last_line	= (ICON_GAME_BTNS_TOP2+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_GAME_BTNS_TOP2)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_GAME_BTNS_LEFT2],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}
		if((line>=ICON_GAME_BTNS_TOP)&&(line<=ICON_GAME_BTNS_TOP1)){
			img_up			= (ico_yfpos[ico_game_screen_map[1]]);
			img_down		= (ico_ytpos[ico_game_screen_map[1]]);
			img_left		= (ico_xfpos[ico_game_screen_map[1]]);
			img_right		= (ico_xtpos[ico_game_screen_map[1]]);
			img_last_line	= (ICON_GAME_BTNS_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_GAME_BTNS_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_GAME_BTNS_LEFT1],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_game_screen_map[3]]);
			img_down		= (ico_ytpos[ico_game_screen_map[3]]);
			img_left		= (ico_xfpos[ico_game_screen_map[3]]);
			img_right		= (ico_xtpos[ico_game_screen_map[3]]);
			img_last_line	= (ICON_GAME_BTNS_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_GAME_BTNS_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_GAME_BTNS_LEFT2],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}
		if(line>=ICON_GAME_BTNS_TOP1){
			img_up			= (ico_yfpos[ico_game_screen_map[2]]);
			img_down		= (ico_ytpos[ico_game_screen_map[2]]);
			img_left		= (ico_xfpos[ico_game_screen_map[2]]);
			img_right		= (ico_xtpos[ico_game_screen_map[2]]);
			img_last_line	= (ICON_GAME_BTNS_TOP1+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_GAME_BTNS_TOP1)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_GAME_BTNS_LEFT1],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_game_screen_map[4]]);
			img_down		= (ico_ytpos[ico_game_screen_map[4]]);
			img_left		= (ico_xfpos[ico_game_screen_map[4]]);
			img_right		= (ico_xtpos[ico_game_screen_map[4]]);
			img_last_line	= (ICON_GAME_BTNS_TOP1+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_GAME_BTNS_TOP1)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_GAME_BTNS_LEFT2],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}

	}
	return false;
}
#pragma GCC pop_options

#pragma GCC push_options
#if COMPOSITE_TV||SOFT_COMPOSITE_TV
#pragma GCC optimize("-Ofast")
#else
#pragma GCC optimize("-Os")
#endif
bool FAST_FUNC(hud_menu_kb)(short int line){
	if(line<1){memset(hud_line,0x10,SCREEN_W);return false;}
	//if(line==(ICON_BAT_BOT+1)){memset(hud_line,0x10,SCREEN_W);return false;}
	if(line==(ICON_MENU_BOTTOM)){memset(hud_line,0x10,SCREEN_W);return false;}
	//if(line==(ICON_MAIN_BTNS_TOP-1)){memset(hud_line,0x10,SCREEN_W);return false;}

	uint16_t addr=0;
	if(current_hud_mode&HM_SHOW_BATTERY){
		if((line>=ICON_BAT_TOP)&&(line<=ICON_BAT_BOT)){
			draw_bufline_text_len(&hud_line[86],(line-ICON_BAT_TOP),&hud_text[0],CL_LT_YELLOW-PALETTE_SHIFT,CL_EMPTY,22);
			if((battery_status&0xF0)>0){
				img_up			= (ico_yfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_down		= (ico_ytpos[ico_battery_map[10+(battery_status>>4)]]);
				img_left		= (ico_xfpos[ico_battery_map[10+(battery_status>>4)]]);
				img_right		= (ico_xtpos[ico_battery_map[10+(battery_status>>4)]]);
				img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
				if(line<img_last_line){
					addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
					memcpy(&hud_line[ICON_CHG_LEFT],&icons[addr+img_left],(img_right-img_left));
				}
			};
			img_up			= (ico_yfpos[ico_battery_map[battery_status&0x0f]]);
			img_down		= (ico_ytpos[ico_battery_map[battery_status&0x0f]]);
			img_left		= (ico_xfpos[ico_battery_map[battery_status&0x0f]]);
			img_right		= (ico_xtpos[ico_battery_map[battery_status&0x0f]]);
			img_last_line	= (ICON_BAT_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_BAT_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_BAT_LEFT],&icons[addr+img_left],(img_right-img_left));
				//return true;
			}
		}
	}	
	if((current_hud_mode&HM_SHOW_MENUBTN)){
		if((line>=ICON_MENU_TOP)&&(line<ICON_MENU_BTNS_TOP1)){
			img_up			= (ico_yfpos[ico_menu_screen_map[0]]);
			img_down		= (ico_ytpos[ico_menu_screen_map[0]]);
			img_left		= (ico_xfpos[ico_menu_screen_map[0]]);
			img_right		= (ico_xtpos[ico_menu_screen_map[0]]);
			img_last_line	= (ICON_MENU_TOP+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_TOP)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_BTNS_LEFT2],&icons[addr+img_left],(img_right-img_left));
				return true;
			}
		}
		if((line>=ICON_MENU_BTNS_TOP1)&&(line<=ICON_MENU_BTNS_TOP2)){
			img_up			= (ico_yfpos[ico_menu_screen_map[1]]);
			img_down		= (ico_ytpos[ico_menu_screen_map[1]]);
			img_left		= (ico_xfpos[ico_menu_screen_map[1]]);
			img_right		= (ico_xtpos[ico_menu_screen_map[1]]);
			img_last_line	= (ICON_MENU_BTNS_TOP1+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_BTNS_TOP1)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_BTNS_LEFT3],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}
		if((line>=ICON_MENU_BTNS_TOP2)&&(line<=ICON_MENU_BTNS_TOP3)){
			img_up			= (ico_yfpos[ico_menu_screen_map[2]]);
			img_down		= (ico_ytpos[ico_menu_screen_map[2]]);
			img_left		= (ico_xfpos[ico_menu_screen_map[2]]);
			img_right		= (ico_xtpos[ico_menu_screen_map[2]]);
			img_last_line	= (ICON_MENU_BTNS_TOP2+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_BTNS_TOP2)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_BTNS_LEFT3],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}
		if((line>=ICON_MENU_BTNS_TOP3)&&(line<=SCREEN_H)){
			img_up			= (ico_yfpos[ico_menu_screen_map[3]]);
			img_down		= (ico_ytpos[ico_menu_screen_map[3]]);
			img_left		= (ico_xfpos[ico_menu_screen_map[3]]);
			img_right		= (ico_xtpos[ico_menu_screen_map[3]]);
			img_last_line	= (ICON_MENU_BTNS_TOP3+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_BTNS_TOP3)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_BTNS_LEFT2],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_menu_screen_map[4]]);
			img_down		= (ico_ytpos[ico_menu_screen_map[4]]);
			img_left		= (ico_xfpos[ico_menu_screen_map[4]]);
			img_right		= (ico_xtpos[ico_menu_screen_map[4]]);
			img_last_line	= (ICON_MENU_BTNS_TOP3+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_BTNS_TOP3)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_BTNS_LEFT],&icons[addr+img_left],(img_right-img_left));
			}
			img_up			= (ico_yfpos[ico_menu_screen_map[5]]);
			img_down		= (ico_ytpos[ico_menu_screen_map[5]]);
			img_left		= (ico_xfpos[ico_menu_screen_map[5]]);
			img_right		= (ico_xtpos[ico_menu_screen_map[5]]);
			img_last_line	= (ICON_MENU_BTNS_TOP3+(img_down-img_up)+1);
			if(line<img_last_line){
				addr=(((line-ICON_MENU_BTNS_TOP3)+img_up)*ICONS_WIDTH);
				memcpy(&hud_line[ICON_MENU_BTNS_LEFT1],&icons[addr+img_left],(img_right-img_left));
			}
			return true;
		}
	}
	return false;
}
#pragma GCC pop_options

#include "util_touch2joy.h"
#include "../lib/bsp/bsp_cst328.h"

const uint16_t touch_buttons_main[KEYS_COUNT][4]={
	{  0,   0,  80,  60}, //menu
    {  0, 200,  80, 240}, //Time
    { 80, 200, 160, 240}, //Game A
    {160, 200, 240, 240}, //Game B
    {240, 200, 320, 240}, //Alarm
};

const uint16_t touch_map_main[KEYS_COUNT] = {
	D_JOY_START,
    SOFTKEY_GAME_A,
    SOFTKEY_GAME_B,
	SOFTKEY_TIME,
    SOFTKEY_ALARM
};

uint16_t decode_touch_main(bsp_cst328_data_t cst328_data){
    uint16_t result = 0;
    for (int i = 0; i < cst328_data.points; i++){
        for (uint8_t key = 0; key < KEYS_COUNT; key++){
            if((cst328_data.coords[i].x>touch_buttons_main[key][0])&&(cst328_data.coords[i].x<touch_buttons_main[key][2])&&
				(cst328_data.coords[i].y>touch_buttons_main[key][1])&&(cst328_data.coords[i].y<touch_buttons_main[key][3])){
                result|=touch_map_main[key];
			}
		}
	}
    printf("decode_touch_main:[%04X]\n",result);
    return result;
}


const uint16_t touch_buttons_game[6][4]={
	{  0,   0,  80,  60}, //menu
    {  0, 120,  80, 180}, //Left UP
    {  0, 180,  80, 240}, //Left DOWN
    {240, 120, 320, 180}, //Right UP
    {240, 180, 320, 240}, //Right DOWN
	{240,  30, 320,  90}, //RESET GAME
};

const uint16_t touch_map_game[6] = {
	D_JOY_START,
    D_JOY_LEFT|D_JOY_UP,
    D_JOY_LEFT|D_JOY_DOWN,
    D_JOY_RIGHT|D_JOY_UP,
    D_JOY_RIGHT|D_JOY_DOWN,
	SOFTKEY_RESET
};

uint16_t decode_touch_game(bsp_cst328_data_t cst328_data){
    uint16_t result = 0;
    for (int i = 0; i < cst328_data.points; i++){
        for (uint8_t key = 0; key < 6; key++){
            if((cst328_data.coords[i].x>touch_buttons_game[key][0])&&(cst328_data.coords[i].x<touch_buttons_game[key][2])&&
				(cst328_data.coords[i].y>touch_buttons_game[key][1])&&(cst328_data.coords[i].y<touch_buttons_game[key][3])){
                result|=touch_map_game[key];
			}
		}
	}
    printf("decode_touch_game:[%04X]\n",result);
    return result;
}


const uint16_t touch_buttons_menu[6][4]={
    {240,   0, 320,  60}, //Left UP
    {240,  60, 320, 120}, //Left DOWN
    {240, 120, 320, 180}, //Right UP
    {240, 180, 320, 240}, //Right DOWN
	{120, 180, 200, 240}, //enter
	{ 40, 180, 120, 240}, //Back
};

const uint16_t touch_map_menu[6] = {
    D_JOY_UP,
	D_JOY_LEFT,
	D_JOY_RIGHT,
    D_JOY_DOWN,
	D_JOY_A,
	D_JOY_START,
};

uint16_t decode_touch_menu(bsp_cst328_data_t cst328_data){
    uint16_t result = 0;
    for (int i = 0; i < cst328_data.points; i++){
        for (uint8_t key = 0; key < 6; key++){
            if((cst328_data.coords[i].x>touch_buttons_menu[key][0])&&(cst328_data.coords[i].x<touch_buttons_menu[key][2])&&
				(cst328_data.coords[i].y>touch_buttons_menu[key][1])&&(cst328_data.coords[i].y<touch_buttons_menu[key][3])){
                result|=touch_map_menu[key];
			}
		}
	}
    printf("decode_touch_menu:[%04X]\n",result);
    return result;
}



const uint16_t touch_sliders[SLIDERS_COUNT][4]={
    {  0,   0, 320, 240}, //Top slider Horizontal
	/*  {  0,   0,  40, 240}, //Left slider Vertical
		{ 40, 210, 280, 240}, //Bottom slider Horizontal
		{280,   0, 320, 240}, //Right slider Vertical
	*/
};

uint32_t t_millis(){
	return us_to_ms(time_us_32());
}

bool check_delay(int m, uint32_t tstamp) {
	if (t_millis() - tstamp > m) {
		tstamp = t_millis();
		return true;
	} else {
		return false;
	}
}

#define TOUCH_NONE		(0x00)
#define TOUCH_BEGIN		(0x10)
#define TOUCH_PROCESS	(0x20)
#define TOUCH_END		(0x40)
#define SKIP_DIV		(5)

uint8_t touch_state = TOUCH_NONE;

uint16_t _oldTouchX, _oldTouchY, _width, _height;
uint32_t _touchdelay;
uint32_t _cntr;

static uint32_t touchLongPress;
static uint16_t direct;


uint16_t _tsDirection(uint16_t x, uint16_t y) {
	int16_t dX = x - _oldTouchX;
	int16_t dY = y - _oldTouchY;
	//printf("DX:[%d]   DY:[%d]\n",dX,dY);
	if (abs(dX) > 20 || abs(dY) > 20) {
		if (abs(dX) > abs(dY)) {
			if (dX > 0) {
				return D_JOY_RIGHT;
			} else {
				return D_JOY_LEFT;
			}
		} else {
			if (dY > 0) {
				return D_JOY_DOWN;
			} else {
				return D_JOY_UP;
			}
		}
	} else {
		return 0;
	}
}

uint16_t decode_touch_all(bsp_cst328_data_t cst328_data, bool is_touch){
    uint16_t touchX, touchY;    
    uint16_t result = 0;
	if((touch_state==TOUCH_NONE)&&(is_touch==true)){
		touch_state=TOUCH_BEGIN;
		return 0;
	}
	if(touch_state==TOUCH_BEGIN){
		touchX = cst328_data.coords[0].x;
		touchY = cst328_data.coords[0].y;
		_oldTouchX = touchX;
		_oldTouchY = touchY;
		touchLongPress=t_millis();
		//printf("START TOUCH:[%8d] _oldTouchX:[%d]  _oldTouchY:[%d]\n",touchLongPress,_oldTouchX,_oldTouchY);
		touch_state=TOUCH_PROCESS;
		_cntr=0;
		return 0;
	}
	if((touch_state==TOUCH_PROCESS)&&(is_touch==true)){
		//printf("PROCESS TOUCH:[%8d] _cntr:[%d]\n",touchLongPress,_cntr);
		_cntr++;

		touchX = cst328_data.coords[0].x;
		touchY = cst328_data.coords[0].y;
		direct = _tsDirection(touchX, touchY);

		switch (direct) {
			case D_JOY_LEFT:
			case D_JOY_RIGHT: {
				touchLongPress=t_millis();
				//printf("L/R:[%04X]\n",direct);
				if((_cntr%SKIP_DIV)>0) return 0;
				break;
			}
			case D_JOY_UP:
			case D_JOY_DOWN: {
				touchLongPress=t_millis();
				//printf("U/D:[%04X]\n",direct);
				if((_cntr%SKIP_DIV)>0) return 0;
				break;
			}
			default:
			break;
		}
		//if(cst328_data.points==2) direct|=D_JOY_SELECT;
		return direct;
	}
	if((touch_state==TOUCH_PROCESS)&&(is_touch==false)){
		touch_state=TOUCH_END;
	}
	if(touch_state==TOUCH_END){
		touch_state=TOUCH_NONE;
		//printf("END TOUCH:[%8d] _oldTouchX:[%d]  _oldTouchY:[%d]\n",t_millis()-touchLongPress,_oldTouchX,_oldTouchY);
		_oldTouchX = 0;
		_oldTouchY = 0;			
		if (direct == 0) {
			uint32_t pressTicks = t_millis()-touchLongPress;
			if( pressTicks < SINGLE_TAP_TICKS*2){
				if(pressTicks > 50){
					//printf("SINGLE TAP\n");
					return D_JOY_A;
				}
			}else{
				//printf("LONG SINGLE TAP\n");
				return D_JOY_START;
			}
		}
		//printf("END TOUCH:[%16d]\n",t_millis()-touchLongPress);
		direct = 0;
	}
	return 0;
}
/*
    for (int i = 0; i < cst328_data.points; i++){
	
	for (uint8_t id = 0; id < SLIDERS_COUNT; id++){
	vx=cst328_data.coords[i].x;
	vy=cst328_data.coords[i].y;
	if(cst328_data.coords[i].pressure>30){
	//if((cst328_data.coords[i].x>touch_sliders[id][0])&&(cst328_data.coords[i].x<touch_sliders[id][2])&&(cst328_data.coords[i].y>touch_sliders[id][1])&&(cst328_data.coords[i].y<touch_sliders[id][3])){
	//if((vx%SLIDERS_SENSE==0)&&(vx<old_x)){result|=D_JOY_LEFT;    old_x=vx;};
	//if((vx%SLIDERS_SENSE==0)&&(vx>old_x)){result|=D_JOY_RIGHT;   old_x=vx;};
	if((vy%SLIDERS_SENSE==0)&&(vy<old_y)){result|=D_JOY_UP;      old_y=vy;};
	if((vy%SLIDERS_SENSE==0)&&(vy>old_y)){result|=D_JOY_DOWN;    old_y=vy;};
	//}
	} else {
	old_x=vx;
	old_y=vy;
	}
	}
	printf("points:%d x:%d y:%d pressure:%d f_count:%d vx:%d vy:%d old_x:%d old_y:%d result:[%04X] timer:%d \n", cst328_data.points, cst328_data.coords[i].x, cst328_data.coords[i].y, cst328_data.coords[i].pressure,f_count,vx,vy,old_x/*[0]/,old_y/*[0]/,result, p_timer);
	busy_wait_ms(100);
	/*if((cst328_data.points==1)&&(cst328_data.coords[0].pressure>SINGLE_TAP_PRESS)&&(old_x==cst328_data.coords[0].x)&&(old_y==cst328_data.coords[0].y)&&(p_timer>SINGLE_TAP)){
	f_count++;
	}
	if(f_count>SINGLE_TAP_CNT){
	f_count = 0;
	result|=D_JOY_A;return result;
	}/
    }
    //old_x/*[0]/=cst328_data.coords[0].x;
    //old_y/*[0]/=cst328_data.coords[0].y;
    return result;
*/

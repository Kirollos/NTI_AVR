/*
 * Ultrasonicprog.c
 *
 * Created: 03/11/2023 10:28:50 AM
 *  Author: Kirollos
 */ 

#include "Ultrasonic_config.h"
#include "Ultrasonic_private.h"
#include "Ultrasonic.h"
#include "../../MCAL/Timer_1/Timer1.h"

volatile static double glo_us_distance = 0;
volatile static u16 numOverflows = 0;
volatile static u32 buff1 = 0;
volatile static u32 buff2 = 0;
volatile static u8 us_flag = 0;

static void cb()
{
	static u8 counter = 0;
	if(!us_flag) return;
	// counter = 0 ( first phase, event on rise)
	switch(counter){
		case 0:
		buff1 = Timer1_ICU_takeReading() + (0x10000 * numOverflows);
		Timer1_ICU_setTrigger(TIMER1_ICES_FALLING_EDGE);
		counter = 1;
		break;
		case 1:
		buff2 = Timer1_ICU_takeReading() + (0x10000 * numOverflows);
		Timer1_ICU_setTrigger(TIMER1_ICES_RISING_EDGE);
		u32 counts = buff2 - buff1;
		double time = (double)counts * (64.0 / F_CPU);
		glo_us_distance = (time/2.0) * 34300.0;
		counter = 0;
		numOverflows = 0;
		us_flag = 0;
		break;
	}
}

static void cb_ovf()
{
	numOverflows++;
}


void H_Ultrasonic_init()
{	
	Timer1_Parameters timer1_parameters;
	timer1_parameters.wave_gen_mode = TIMER1_WGM_NORMAL;
	timer1_parameters.ic_edge_select = TIMER1_ICES_RISING_EDGE;
	Timer1_Init(&timer1_parameters);
	Timer1_ICU_setCallBack(cb);
	Timer1_setCallBack(cb_ovf);
	Timer1_EnableInt(TIMER1_INT_OVF);
	Timer1_EnableInt(TIMER1_INT_IC);
	Timer1_start(TIMER1_CLK_DIV_64);
		//		H_LCD_void_gotoXY(1,0);
		//		H_LCD_void_sendString("Distance: ");
		//		H_LCD_void_sendDouble(glo_us_distance);
		//		H_LCD_void_sendString(" cm");
		//		u8 pos_x,pos_y;
		//		H_LCD_void_getXY(&pos_x, &pos_y);
		//		for(u8 i = pos_y; i < 20; i++)
		//			H_LCD_void_sendData(' ');
		//H_LCD_void_gotoXY(3,0);
		//H_LCD_void_sendDouble(buff3);
}

void H_Ultrasonic_start()
{
	Timer1_EnableInt(TIMER1_INT_OVF);
	Timer1_EnableInt(TIMER1_INT_IC);
	Timer1_start(ULTRASONIC_TIMER_PRESCALER);
}

void H_Ultrasonic_stop()
{
	Timer1_DisableInt(TIMER1_INT_OVF);
	Timer1_DisableInt(TIMER1_INT_IC);
	Timer1_stop();
}
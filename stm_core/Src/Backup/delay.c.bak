/*
 * delay.c
 *
 *  Created on: 2022年6月16日
 *      Author: a3352
 */
#include "delay.h"

static uint32_t fac_us=0;

void delay_init(void)
{

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	fac_us=72;

}

void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;
	ticks=nus*fac_us;
	told=SysTick->VAL;
	while(1)
	{
		tnow=SysTick->VAL;
		if(tnow!=told)
		{
			if(tnow<told)tcnt+=told-tnow;
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;
		}
	};
}


void delay_ms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++) delay_us(1000);
}

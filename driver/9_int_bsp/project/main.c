#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_key.h"
#include "bsp_beep.h"

int main(void)
{
//	int i = 0;
//	int keyvalue;
//	unsigned char beep_state = OFF;



	unsigned char led_state = OFF;
	int_init();				/* 初始化中断 */
	imx6u_clkinit();		/* 初始化系统时钟 */
	clk_enable();   		/* 使能外设时钟 */
    led_init();     		/* 初始化LED */
	beep_init();			/* 初始化蜂鸣器 */ 
	key_init();				/* 初始化按键 */
	exti_init();			/* 初始化中断 */

    /* 初始化LED */
    
    /* 设置LED闪烁 */
    while(1) {
		

		led_state = !led_state;
		led_switch(LED0, led_state);
		delay(500);

		#if 0
		keyvalue = key_getvalue();
		if(keyvalue)	/*如果为正*/
		{
			switch(keyvalue)
			{		
				case KEY0_VALUE:
					beep_state = !beep_state;
					beep_switch(beep_state);
					break;

			}
		}

		i++;
		if(i == 50) /*500ms*/
		{
			i = 0;
			led_state = !led_state;
			led_switch(LED0, led_state);
		}
		delay(10);
		#endif
    }
    return 0;
}

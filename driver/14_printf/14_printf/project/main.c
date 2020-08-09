#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_epit.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"
#include "stdio.h"

int main(void)
{



	int_init();				/* 初始化中断 */
	imx6u_clkinit();		/* 初始化系统时钟 */
	delay_init();			/* 初始化延时函数 */
	uart_init();			/* 初始化串口，波特率115200 */
	clk_enable();   		/* 使能外设时钟 */
    led_init();     		/* 初始化LED */
	beep_init();			/* 初始化蜂鸣器 */ 
	keyfilter_init();		/* 初始化按键过滤 */
	

    printf("%d的十六进制为：%#x\r\n",100,100);
    /* 设置LED闪烁 */
    while(1) 
	{		
		#ifdef MANUAL_BAUD_RATE
		unsigned char a;
		static unsigned char led_state = OFF;	
		puts("请输入1个字符:");
		a = getc();
		putc(a);
		puts("\r\n");

		puts("您输入的字符为:");
		putc(a);
		puts("\r\n");
		led_state = !led_state;
		led_switch(LED0, led_state);
		delay_us(500);
		#else
		
		int a, b;		
		printf("请输入两个整数，使用空格隔开:");
		scanf("%d %d",&a, &b);
		printf("\r\n数据%d + %d = %d\r\n\r\n",a, b, a+b);

		#endif

    }
    return 0;
}

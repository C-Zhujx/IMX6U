#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"


int main(void)
{
    clk_enable();   /* 使能外设时钟 */
    led_init();     /* 初始化LED */
    /* 初始化LED */
    
    /* 设置LED闪烁 */
    while(1) {
        led_on();
        delay(1000);

        led_off();
        delay(1000);
    }
    return 0;
}
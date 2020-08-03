#include "main.h"


/* 使能外部时钟 */
void clk_enable(void)
{
    CCM_CCGR0 = 0XFFFFFFFF;
    CCM_CCGR1 = 0XFFFFFFFF;
    CCM_CCGR2 = 0XFFFFFFFF;
    CCM_CCGR3 = 0XFFFFFFFF;
    CCM_CCGR4 = 0XFFFFFFFF;
    CCM_CCGR5 = 0XFFFFFFFF;
    CCM_CCGR6 = 0XFFFFFFFF;
}



/* 初始化 LED */
void led_init(void)
{
    SW_MUX_GPIO1_IO03 = 0X5;        /* 复用为GPIO1_IO03 */
    SW_PAD_GPIO1_IO03 = 0X10B0;     /* 配置GPIO1_IO03的电气属性 */

    /* GPIO 初始化 */
    GPIO1_GDIR = 0X8;               /* 设置为输出 */
    GPIO1_DR   = 0X0;               /* 点亮LED灯 */

}

/* 短延时函数 */
void delay_short(volatile unsigned int n)
{
    while(n--){}
}

/* 延时，一次循环大概是1ms， 在主频396MHz
 * n: 延时Ms数
 */
void delay(volatile unsigned int n)
{
    while(n--)
    {
        delay_short(0x7ff);
    }
}

/* 打开LED灯 */
void led_on(void)
{
    GPIO1_DR &= ~(1<<3);  /*BIT3清零*/
}

/* 关闭LED灯 */
void led_off(void)
{
    GPIO1_DR |= (1<<3);
}


int main(void)
{
    clk_enable();       /* 初始换时钟 */
    led_init();         /* 初始化led */
    while(1)
    {
        led_on();
        delay(500);

        led_off();
        delay(500);
    }

    return 0;
}



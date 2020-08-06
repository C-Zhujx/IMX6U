#include "bsp_keyfilter.h"
#include "bsp_gpio.h"
#include "bsp_beep.h"
#include "bsp_int.h"

/* keyfilter初始化 */
void keyfilter_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);     /*复用为GPIO1_18*/
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0XF080);

    /* 初始化GPIO */
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn,gpio1_16_31_irqhandler, NULL);
    gpio_enableint(GPIO1, 18);

    /* 初始化定时器 定时周期10ms */
    filtertimer_init(66000000/100);     
}


/* 初始化EPIT1定时器 */
void filtertimer_init(unsigned int value)
{
    EPIT1->CR = 0;
    EPIT1->CR = (1<<1) | (1<<2) | (1<<3) | (1<<24); /* 默认的分配值写0，即1分频 */

    EPIT1->LR = value;          /* 加载寄存器，相当于倒计数值 */
    EPIT1->CMPR = 0;            /* 比较寄存器 */

    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqhandler(EPIT1_IRQn, filtertimer_irqhandler, NULL);

}

/* 关闭EPIT1定时器 */
void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

/* 重启EPIT1定时器 */
void filtertimer_restart(unsigned int value)
{
    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = value;
    EPIT1->CR |= (1 << 0);
}


/* EPIT1 定时器中断处理函数 */
void filtertimer_irqhandler(unsigned int gicciar, void *param)
{
    static unsigned char state = OFF;

    if(EPIT1->SR & 0X1)     /* 判断是否开启了定时器中断 */
    {
        /* 关闭定时器 */
        filtertimer_stop();
        if(gpio_pinread(GPIO1, 18) == 0)    /* 判断按键是否按下 */
        {
            state = !state;
            beep_switch(state);
        }
    }
    EPIT1->SR |= 1;     /* 清除中断标志位 */
}

void gpio1_16_31_irqhandler(unsigned int gicciar, void *param)
{
    /* 开启定时器 */
    filtertimer_restart(66000000/100);

    /* 清除中断标志位 */
    gpio_clearintflags(GPIO1, 18);
}
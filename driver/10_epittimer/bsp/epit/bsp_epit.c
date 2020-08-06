#include "bsp_epit.h"
#include "bsp_int.h"
#include "bsp_led.h"


/* 初始化EPIT1 */
void epit_init(unsigned int frac, unsigned int value)
{
    if(frac > 4095)
        frac = 4095;
    
    EPIT1->CR = 0;      /*先清零*/

    /*设置CR寄存器*/
    EPIT1->CR = (1<<24) | (frac<<4) | (1<<3) | (1<<2) | (1<<1);

    /* 设置LR 加载寄存器 */
    EPIT1->LR = value;
    
    /* 设置CMPR比较寄存器 */
    EPIT1->CMPR = 0;

    /* 初始化EPIT1_IRQ中断 */
    GIC_EnableIRQ(EPIT1_IRQn);

    system_register_irqhandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqhandler, NULL);

    EPIT1->CR |= 1;         /* 使能EPIT */
}




/* EPIT1的中断处理函数 */
void epit1_irqhandler(void)
{
    static uint8_t state = 0;

    state = !state;
    if(EPIT1->SR & 0X1)         /* 判断是否发生了中断 1：发生中断 */
    {
        led_switch(LED0, state);
    }
    
    /* 清除中断标志位 */
    EPIT1->SR |= (1 << 0);

}

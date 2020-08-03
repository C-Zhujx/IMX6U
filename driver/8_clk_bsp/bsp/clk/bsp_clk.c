#include "bsp_clk.h"
#include "cc.h"
/* 使能外设时钟 */
void clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}


void imx6u_clkinit(void)
{
    unsigned int reg = 0;
    if(((CCM->CCSR >> 2) & 0X1 ) == 0)
    {
        CCM->CCSR &= ~(1 << 8);
        CCM->CCSR |= (1 << 2);          //设置成step_clk
    }



    /*528 * 2 = 24 * DEV_SEL / 2 ---> DEV_SEL = 88
     *DEV_SEL = CCM_ANALOG_PLL_ARM[DIV_SELECT] 
     */
    
    CCM_ANALOG->PLL_ARM = (((88 << 0) & 0X7F) | (1 << 13) );
//	CCM_ANALOG->PLL_ARM &= ~(3 << 14);
    CCM->CACRR = 1;
    CCM->CCSR &= ~(1 << 2);

	//PLL2固定为528MHZ，PLL3固定为480MHz
	//初始化PLL2_PFD0-PFD3 
	reg = CCM_ANALOG->PFD_528;
    reg &= ~(0x3f3f3f3f);
    reg |= (27 << 0);           /* 352 */
    reg |= (16 << 8);           /* 594 */
    reg |= (24 << 16);          /* 396 */      
    reg |= (32 << 24);          /* 297 */    
    CCM_ANALOG->PFD_528 = reg;

	//初始化PLL3_PFD0-PFD3
    reg = 0;
	reg = CCM_ANALOG->PFD_480;
    reg &= ~(0x3f3f3f3f);
    reg |= (12 << 0);           /* 720 */      
    reg |= (16 << 8);           /* 540 */
    reg |= (17 << 16);          /* 508.2 */
    reg |= (19 << 24);          /* 454.7 */
    CCM_ANALOG->PFD_480 = reg;

    

    CCM->CBCMR &= ~(3 << 18);
    CCM->CBCMR |= (1 << 18);
    CCM->CBCDR &= ~(1 << 25);
    while(CCM->CDHIPR & (1 << 5));  /* 等待握手信号完成 */

#if 0
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= (2 << 10);        /*3 分频*/
    while(CCM->CDHIPR & (1 << 1));  /* 等待握手信号完成 */
#endif

    /* IPG_CLK_ROOT */
    CCM->CBCDR &= ~(3 << 8);
    CCM->CBCDR |= (1 << 8);

    /* PERCLK_CLK_ROOT */
    CCM->CSCMR1 &= ~(1 << 6);
    CCM->CSCMR1 &= ~(0X3F << 0);
    
}

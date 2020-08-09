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
    uint32_t reg = 0;
    /*判断CCSR[PLL1_SW_CLK_SEL]是否是0，等于0，则设置为1*/
    if(((CCM->CCSR >> 2) & 0X1) == 0)
    {
        CCM->CCSR |= (1 << 2);
        CCM->CCSR &= ~(1 << 8);             /*选择备用时钟源24MHz*/
    }

    /* 设置PLL1 */
    CCM_ANALOG->PLL_ARM = ((88 << 0)&0X7F) | (1 << 13);            /*1052MHz*/
    CCM->CACRR &= (7 << 0);
    CCM->CACRR |= (1 << 0);                 /*2分频*/
    CCM->CCSR &= ~(1 << 2);                  /*选择pll1_main_clk*/



    /*PLL2 PLL3的PFD0-3的设置 */
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0X3F3F3F3F);
    reg |= (12 << 0);               /*PFD0*/
    reg |= (16 << 8);               /*PFD1*/
    reg |= (17 << 16);              /*PFD2*/
    reg |= (19 << 24);              /*PFD3*/
    CCM_ANALOG->PFD_480 = reg;

    reg = 0;
    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0X3F3F3F3F);
    reg |= (27 << 0);
    reg |= (16 << 8);
    reg |= (24 << 16);
    CCM_ANALOG->PFD_528 |= (32 << 24);
    CCM_ANALOG->PFD_528 = reg;



    /*
     *AHB_CLK_ROOT=132MHz
     *IPG_CLK_ROOT=66MHz
     *PERCLK_CLK_ROOT=66MHz
     */ 

    CCM->CBCMR &= ~(0X3 << 18);
    CCM->CBCMR |= (0X1 << 18);
    CCM->CBCDR &= ~(1 << 25);
    while( (CCM->CDHIPR >> 5) & 0X1) {}

#if 0
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= (2 << 10);        /*3 分频*/
    while(CCM->CDHIPR & (1 << 1));  /* 等待握手信号完成 */
#endif

    CCM->CBCDR &= ~(0X3 << 8);
    CCM->CBCDR |= (1 << 8);


    CCM->CSCMR1 &= ~(1 << 6);
    CCM->CSCMR1 &= ~(0X3F << 0);

    /* 设置UART时钟源频率80MHz */
    CCM->CSCDR1 &= ~(1 << 6);       /* UART时钟源选择PLL3 80MHz */
    CCM->CSCDR1 &= ~0X3F;           /* UART时钟1分频 */
}

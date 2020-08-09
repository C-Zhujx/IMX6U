#include "bsp_uart.h"
#include "bsp_gpio.h"

/*********************************************************
 * @function        : uart_init
 * @description     : 初始化串口1，波特率为115200
 * @param           : 无
 * @return          : 无
 *********************************************************/ 
void uart_init(void)
{
    uart_io_init();

    /* 初始化UAR1 */
    uart_disable(UART1);                /* 先关闭UART1 */
    uart_softreset(UART1);              /* 软件复位UART1 */

    UART1->UCR1 = 0;                    /* 先清零 */
    UART1->UCR1 &= ~(1 << 14);          /* 关闭自动检测波特率 */
    
    UART1->UCR2 = 0;
    UART1->UCR2 |= (1 << 14) | (1 << 5) | (1 << 2) | (1 << 1);
    
    UART1->UCR3 |= (1 << 2);            /* 此位必须置1 */

    /* 设置波特率 */
    /* 波特率公式 Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)) */
    /* Ref=80000000 UBMR=3124 UBIR=71 */
    #ifdef MANUAL_BAUD_RATE
    UART1->UFCR = (5 << 7);
    UART1->UBMR = 3124;
    UART1->UBIR = 71;
    #else
    uart_setbaudrate(UART1, 115200, 80000000);
    #endif
    /* 最后的最后，一定要使能UART */
    uart_enable(UART1);
}

/*********************************************************
 * @function        : uart_io_init
 * @description     : 初始化串口1所使用的IO引脚
 * @param           : 无
 * @return          : 无
 *********************************************************/ 
void uart_io_init(void)
{
    /* 1 初始化IO复用 
     * UART1_RXD -> UART1_RX_DATA
     * UART1_TXD -> UART1_TX_DATA
     */
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);      /* 复用为UART1_TX */
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);      /* 复用为UART1_RX */

    /* 2 配置UART1_TX_DATA, UART1_RX_DATA的IO属性
     * bit 16:0     HYX关闭
     * bit 15:14    00默认100K下拉
     * bit 13       0 keeper功能
     * bit 12       1 pull/keeper使能
     * bit 11       0 关闭开路输出
     * bit 7:6      10速度为100MHz
     * bit 5:3      110 驱动能力R0/6
     * bit 0        0 底转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0X10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0X10B0);
}

/*********************************************************
 * @function        : uart_disable
 * @description     : 禁止UART
 * @param           : UART_Type类型指针
 * @return          : 无
 *********************************************************/ 
void uart_disable(UART_Type *base)
{
    base->UCR1 &= ~(1 << 0);
}

/*********************************************************
 * @function        : uart_enable
 * @description     : 使能UART
 * @param           : UART_Type类型指针
 * @return          : 无
 *********************************************************/ 
void uart_enable(UART_Type *base)
{
    base->UCR1 |= (1 << 0);
}

/*********************************************************
 * @function        : uart_softreset
 * @description     : 使能UART
 * @param           : UART_Type类型指针
 * @return          : 无
 *********************************************************/ 
void uart_softreset(UART_Type *base)
{
    base->UCR2 &= ~(1 << 0);
    while((base->UCR2 & 0X1) == 0);      /* 等待复位完成 */
}

/*********************************************************
 * @function        : putc
 * @description     : 发送一个字节
 * @param           : c 发送的一个字节
 * @return          : 无
 *********************************************************/ 
void putc(unsigned char c)
{
    while(((UART1->USR2 >> 3) & 0X1) == 0);     /* 等待上一个字节发送完成 */
    UART1->UTXD = c & 0xff;                     /* 发送数据 */
}

/*********************************************************
 * @function        : puts
 * @description     : 发送一个字符串
 * @param           : str 字符串指针
 * @return          : 无
 *********************************************************/ 
void puts(char *str)
{
    char *p;
    p = str;
    while(*p)
    {
        putc(*p++);
    }
}

/*********************************************************
 * @function        : getc
 * @description     : 接收一个字节
 * @param           : 
 * @return          : 接收的一个字节
 *********************************************************/ 
unsigned char getc(void)
{
    while(((UART1->USR2) & 0X01) == 0);     /* 等待接收 */
    return UART1->URXD;
}

/*********************************************************
 * @function        : raise
 * @description     : 防止编译器报错
 * @param           : sig_nr
 * @return          : 无
 *********************************************************/ 
void raise(int sig_nr)
{

}

/*********************************************************
 * @function        : uart_setbaudrate
 * @description     : 根据波特率，自动计算出UFCR UBIR UBMR应该赋的值
 * @param           : base UARTx
 *                  : baudrate  波特率
 *                  : srcclock_hz 时钟频率
 * @return          : 无
 *********************************************************/ 
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz)
{
    uint32_t numerator = 0u;		//分子
    uint32_t denominator = 0U;		//分母
    uint32_t divisor = 0U;
    uint32_t refFreqDiv = 0U;
    uint32_t divider = 1U;
    uint64_t baudDiff = 0U;
    uint64_t tempNumerator = 0U;
    uint32_t tempDenominator = 0u;

    /* get the approximately maximum divisor */
    numerator = srcclock_hz;
    denominator = baudrate << 4;
    divisor = 1;

    while (denominator != 0)
    {
        divisor = denominator;
        denominator = numerator % denominator;
        numerator = divisor;
    }

    numerator = srcclock_hz / divisor;
    denominator = (baudrate << 4) / divisor;

    /* numerator ranges from 1 ~ 7 * 64k */
    /* denominator ranges from 1 ~ 64k */
    if ((numerator > (UART_UBIR_INC_MASK * 7)) || (denominator > UART_UBIR_INC_MASK))
    {
        uint32_t m = (numerator - 1) / (UART_UBIR_INC_MASK * 7) + 1;
        uint32_t n = (denominator - 1) / UART_UBIR_INC_MASK + 1;
        uint32_t max = m > n ? m : n;
        numerator /= max;
        denominator /= max;
        if (0 == numerator)
        {
            numerator = 1;
        }
        if (0 == denominator)
        {
            denominator = 1;
        }
    }
    divider = (numerator - 1) / UART_UBIR_INC_MASK + 1;

    switch (divider)
    {
        case 1:
            refFreqDiv = 0x05;
            break;
        case 2:
            refFreqDiv = 0x04;
            break;
        case 3:
            refFreqDiv = 0x03;
            break;
        case 4:
            refFreqDiv = 0x02;
            break;
        case 5:
            refFreqDiv = 0x01;
            break;
        case 6:
            refFreqDiv = 0x00;
            break;
        case 7:
            refFreqDiv = 0x06;
            break;
        default:
            refFreqDiv = 0x05;
            break;
    }
    /* Compare the difference between baudRate_Bps and calculated baud rate.
     * Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)).
     * baudDiff = (srcClock_Hz/divider)/( 16 * ((numerator / divider)/ denominator).
     */
    tempNumerator = srcclock_hz;
    tempDenominator = (numerator << 4);
    divisor = 1;
    /* get the approximately maximum divisor */
    while (tempDenominator != 0)
    {
        divisor = tempDenominator;
        tempDenominator = tempNumerator % tempDenominator;
        tempNumerator = divisor;
    }
    tempNumerator = srcclock_hz / divisor;
    tempDenominator = (numerator << 4) / divisor;
    baudDiff = (tempNumerator * denominator) / tempDenominator;
    baudDiff = (baudDiff >= baudrate) ? (baudDiff - baudrate) : (baudrate - baudDiff);

    if (baudDiff < (baudrate / 100) * 3)
    {
        base->UFCR &= ~UART_UFCR_RFDIV_MASK;
        base->UFCR |= UART_UFCR_RFDIV(refFreqDiv);
        base->UBIR = UART_UBIR_INC(denominator - 1); //要先写UBIR寄存器，然后在写UBMR寄存器，3592页 
        base->UBMR = UART_UBMR_MOD(numerator / divider - 1);
        //base->ONEMS = UART_ONEMS_ONEMS(srcclock_hz / (1000 * divider));
    }

}

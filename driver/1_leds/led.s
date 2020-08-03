.global _start @全局标号

_start:
    /* 使能所有外设时钟 */
    ldr r0, =0x020c4068     @CCGR0
    ldr r1, =0xffffffff     @要向CCGR0写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR0中

    ldr r0, =0x020c406c     @CCGR1
    ldr r1, =0xffffffff     @要向CCGR1写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR1中

    ldr r0, =0x020c4070     @CCGR2
    ldr r1, =0xffffffff     @要向CCGR2写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR2中

    ldr r0, =0x020c4074     @CCGR3
    ldr r1, =0xffffffff     @要向CCGR3写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR3中

    ldr r0, =0x020c4078     @CCGR4
    ldr r1, =0xffffffff     @要向CCGR4写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR4中

    ldr r0, =0x020c407c     @CCGR5
    ldr r1, =0xffffffff     @要向CCGR5写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR5中

    ldr r0, =0x020c4080     @CCGR6
    ldr r1, =0xffffffff     @要向CCGR6写入的数据
    str r1, [r0]            @将0xffffffff写入到CCGR6中


    /* 配置GPIO1_IO03 PIN的服用为GPIO，也就是配置
     * IOMUX_SW_MUX_CTL_PAD_GPIO1_IO03 = 5
     * IOMUX_SW_MUX_CTL_PAD_GPIO1_IO03寄存器地址为0x020e0068
     */
    ldr r0, =0x020e0068    @IOMUX_SW_MUX_CTL_PAD_GPIO1_IO03地址
    ldr r1, =0x5            
    str r1, [r0] 

    /* 配置GPIO1_IO03的电气属性 即寄存器：
     * IOMUX_SW_PAD_CTL_PAD_GPIO1_IO03
     * 地址为 0x020e02f4
     * 
     * bit0:        0 低速率
     * bit5:3       110 R0/6驱动能力
     * bit7:6       10 100MHz速率
     * bit11:       0 关闭开路输出
     * bit12:       1 使能pull/kepper
     * bit13:       0 kepper
     * bit15:14     00 100K下拉
     * bit16:       0 关闭
     */
    
    ldr r0, =0x020e02f4
    ldr r1, =0x10b0
    str r1, [r0]

    /* 设置GPIO
     * 设置GPIO1_GDIR寄存器，设置GPIO1_GPIO03为输出
     * GPIO1_GDIR寄存器地址为0x0209c004,设置GPIO_GDIR寄存器bit3为1
     * 设置GPIO1_IO03为输出
     */
    ldr r0, =0x0209c004
    ldr r1, =0x8
    str r1, [r0]

    /* 打开led, 即设置GPIO1_IO03为0 
     * GPIO1_DR寄存器地址为0x0209c000
     */
    ldr r0, =0x0209c000
    ldr r1, =0
    str r1, [r0]

loop:
    b loop

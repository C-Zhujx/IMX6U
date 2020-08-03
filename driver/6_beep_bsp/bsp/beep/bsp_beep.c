#include "bsp_beep.h"
#include "cc.h"



void beep_init(void)
{
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10b0);

	/*GPIO 初始化*/
	GPIO5->GDIR |= (1<<1);	/*设置为输出*/
	GPIO5->DR |= (1<<1);
}


void beep_switch(int status)
{
	if(status == ON)
		GPIO5->DR &= ~(1<<1);		/*输出低电平 打开蜂鸣器*/
	if(status == OFF)
		GPIO5->DR |= (1<<1);		/*输出高电平，关闭蜂鸣器*/

}

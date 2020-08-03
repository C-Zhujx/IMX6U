#include "bsp_beep.h"
#include "cc.h"
#include "bsp_gpio.h"


void beep_init(void)
{
	gpio_pin_config_t beep_config;
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10b0);

	/*GPIO 初始化*/
	beep_config.direction = kGPIO_DigitalOutput;
	beep_config.outputLogic = 1;

	gpio_init(GPIO5, 1, &beep_config);
}


void beep_switch(int status)
{
	if(status == ON)
		gpio_pinwrite(GPIO5, 1, 0);/*输出低电平 打开蜂鸣器*/

	if(status == OFF)
		gpio_pinwrite(GPIO5, 1, 1);		/*输出高电平，关闭蜂鸣器*/

}

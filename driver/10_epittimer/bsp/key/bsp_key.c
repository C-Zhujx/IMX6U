#include "bsp_key.h"
#include "cc.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"

void key_init(void)
{
	gpio_pin_config_t key_config;
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0XF080);

	key_config.direction = kGPIO_DigitalInput;
	gpio_init(GPIO1, 18, &key_config);		/*设置为输入*/
}



int read_key(void)
{
	return gpio_pinread(GPIO1, 18);
}
int key_getvalue(void)
{
	int ret = 0;
	static unsigned char release = 1;	/*1表示按键释放*/
	
	if((release == 1) && (read_key() == 0))
	{
		delay(10);
		release = 0;
		if(read_key() == 0)
			ret = KEY0_VALUE;
	}	
	else if( read_key() == 1)
	{
		ret = KEY_NONE;
		release = 1;
	}
	return ret;

}

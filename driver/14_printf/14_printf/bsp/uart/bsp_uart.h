#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "imx6u.h"

//#define MANUAL_BAUD_RATE



void raise(int sig_nr);
unsigned char getc(void);
void puts(char *str);
void putc(unsigned char c);
void uart_softreset(UART_Type *base);
void uart_enable(UART_Type *base);
void uart_disable(UART_Type *base);
void uart_io_init(void);
void uart_init(void);
void uart_setbaudrate(UART_Type *base, unsigned int baudrate, unsigned int srcclock_hz);
#endif 

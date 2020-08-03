
CROSS_COMPILE	?= arm-linux-gnueabihf-
TARGET			?= ledc

CC				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY			:= $(CROSS_COMPILE)objcopy
OBJDUMP			:= $(CROSS_COMPILE)objdump

INCUDIRS		:= 	imx6u \
					bsp/clk \
					bsp/led \
					bsp/delay

SRCDIRS			:= 	project \
					bsp/clk \
					bsp/led \
					bsp/delay

INCLUDE			:= $(patsubst %, -I %, $(INCUDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))



print:
	@echo INCLUDE=$(INCLUDE)
	@echo SFILES=$(SFILES)
	@echo CFILES=$(CFILES)


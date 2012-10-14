To use this driver:

1. Add in your halconf.h:
	a) #define HAL_USE_TOUCHPAD		TRUE

2. To your makefile add the following lines:
	include $(LCDLIB)/drivers/touchpad/STMPE811/touchpad_lld.mk


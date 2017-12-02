#ifndef __DELAY_CONF_H__
#define __DELAY_CONF_H__

#include <utils.h>

#define DELAY_SYS_INIT_LED_ON       DELAY_MS(90)   //系统初始化指示灯亮延时
#define DELAY_SYS_INIT_LED_OFF      DELAY_MS(10)   //系统初始化指示灯灭延时
#define DELAY_SYS_RUN_LED_ON        DELAY_MS(960) //系统正常运行指示灯亮延时
#define DELAY_SYS_RUN_LED_OFF       DELAY_MS(40)  //系统正常运行指示灯灭延时
#define DELAY_SYS_FAULT_LED_ON      DELAY_MS(40)   //系统故障运行指示灯亮延时
#define DELAY_SYS_FAULT_LED_OFF     DELAY_MS(960)  //系统故障运行指示灯灭延时

#define DELAY_SYS_SLEEP_LED			DELAY_MS(1000) //系统睡眠指示灯延时1s


#endif  


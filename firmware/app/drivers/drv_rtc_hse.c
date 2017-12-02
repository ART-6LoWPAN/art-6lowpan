/*
 * File      : drv_rtc_hse.c (RT-thread stm32f4xx RTC using HSE driver)
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2016, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-05-23     armink       the first version
 */


#include <string.h>
#include <time.h>
#include <rtthread.h>
#include <board.h>

#include <drivers/rtc.h>

static struct rt_device rtc;
static rt_err_t rt_rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* Open Interrupt */
    }

    return RT_EOK;
}

static rt_size_t rt_rtc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    return 0;
}

static rt_err_t rt_rtc_control(rt_device_t dev, int cmd, void *args)
{
    time_t *time;
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;
    struct tm time_temp;

    RT_ASSERT(dev != RT_NULL);
    memset(&time_temp, 0, sizeof(struct tm));

    switch (cmd)
    {
    case RT_DEVICE_CTRL_RTC_GET_TIME:
        time = (time_t *)args;

        /* Get the current Time */
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
        /* Years since 1900 : 0-99 range */
        time_temp.tm_year = RTC_DateStructure.RTC_Year + 2000 - 1900;
        /* Months *since* january 0-11 : RTC_Month_Date_Definitions 1 - 12 */
        time_temp.tm_mon = RTC_DateStructure.RTC_Month - 1;
        /* Day of the month 1-31 : 1-31 range */
        time_temp.tm_mday = RTC_DateStructure.RTC_Date;
        /* Hours since midnight 0-23 : 0-23 range */
        time_temp.tm_hour = RTC_TimeStructure.RTC_Hours;
        /* Minutes 0-59 : the 0-59 range */
        time_temp.tm_min = RTC_TimeStructure.RTC_Minutes;
        /* Seconds 0-59 : the 0-59 range */
        time_temp.tm_sec = RTC_TimeStructure.RTC_Seconds;

        *time = mktime(&time_temp);
        break;

    case RT_DEVICE_CTRL_RTC_SET_TIME:
    {
        const struct tm* time_new;
        time = (time_t *)args;
        time_new = localtime(time);

        /* 0-99 range              : Years since 1900 */
        RTC_DateStructure.RTC_Year = time_new->tm_year + 1900 - 2000;
        /* RTC_Month_Date_Definitions 1 - 12 : Months *since* january 0-11 */
        RTC_DateStructure.RTC_Month = time_new->tm_mon + 1;
        /* 1-31 range : Day of the month 1-31 */
        RTC_DateStructure.RTC_Date = time_new->tm_mday;
        /* 1 - 7 : Days since Sunday (0-6) */
        RTC_DateStructure.RTC_WeekDay = time_new->tm_wday + 1;
        /* 0-23 range : Hours since midnight 0-23 */
        RTC_TimeStructure.RTC_Hours = time_new->tm_hour;
        /* the 0-59 range : Minutes 0-59 */
        RTC_TimeStructure.RTC_Minutes = time_new->tm_min;
        /* the 0-59 range : Seconds 0-59 */
        RTC_TimeStructure.RTC_Seconds = time_new->tm_sec;

        /* Allow access to RTC */
        PWR_BackupAccessCmd(ENABLE);

        /* Set Current Time and Date */
        RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
    }
    break;
    }

    return RT_EOK;
}

static int RTC_Configuration(void)
{
    RTC_TimeTypeDef   RTC_TimeStructure;
    RTC_DateTypeDef   RTC_DateStructure;
    RTC_InitTypeDef   RTC_InitStructure;

#define INIT_FLAG            0x5050

    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div8);
    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);
    /* It's the first configuration */
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != INIT_FLAG) {
        /* Calendar Configuration */
        RTC_InitStructure.RTC_AsynchPrediv = 100 - 1;
        RTC_InitStructure.RTC_SynchPrediv = 10000 - 1;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
        RTC_Init(&RTC_InitStructure);
        /* Set the Time */
        RTC_TimeStructure.RTC_Hours = 0x08;
        RTC_TimeStructure.RTC_Minutes = 0x00;
        RTC_TimeStructure.RTC_Seconds = 0x00;
        RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
        /* Set the Date */
        RTC_DateStructure.RTC_Month = RTC_Month_January;
        RTC_DateStructure.RTC_Date = 1;
        RTC_DateStructure.RTC_Year = 16;
        RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday;
        RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
        /* Set initialized complete flag */
        RTC_WriteBackupRegister(RTC_BKP_DR0, INIT_FLAG);
    }

    return 0;
}

clock_t clock(void) {
    return rt_tick_get();
}

int rt_hw_rtc_init(void)
{
    rtc.type    = RT_Device_Class_RTC;

    RTC_Configuration();

    /* register rtc device */
    rtc.init    = RT_NULL;
    rtc.open    = rt_rtc_open;
    rtc.close   = RT_NULL;
    rtc.read    = rt_rtc_read;
    rtc.write   = RT_NULL;
    rtc.control = rt_rtc_control;

    /* no private */
    rtc.user_data = RT_NULL;

    rt_device_register(&rtc, "rtc", RT_DEVICE_FLAG_RDWR);

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_rtc_init);

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#include <finsh.h>
static void date(uint8_t argc, char **argv) {
    if (argc == 1) {
        time_t now;
        /* output current time */
        now = time(RT_NULL);
        rt_kprintf("%s", ctime(&now));
    } else if (argc == 7) {
        /* set time and date */
        uint16_t year;
        uint8_t month, day, hour, min, sec;
        year = atoi(argv[1]);
        month = atoi(argv[2]);
        day = atoi(argv[3]);
        hour = atoi(argv[4]);
        min = atoi(argv[5]);
        sec = atoi(argv[6]);
        if (year > 2099 || year < 2000) {
            rt_kprintf("year is out of range [2000-2099]\n");
            return;
        }
        if (month == 0 || month > 12) {
            rt_kprintf("month is out of range [1-12]\n");
            return;
        }
        if (day == 0 || day > 31) {
            rt_kprintf("day is out of range [1-31]\n");
            return;
        }
        if (hour > 23) {
            rt_kprintf("hour is out of range [0-23]\n");
            return;
        }
        if (min > 59) {
            rt_kprintf("minute is out of range [0-59]\n");
            return;
        }
        if (sec > 59) {
            rt_kprintf("secound is out of range [0-59]\n");
            return;
        }
        set_time(hour, min, sec);
        set_date(year, month, day);
    } else {
        rt_kprintf("please inpput: date [year month day hour min sec] or date\n");
        rt_kprintf("e.g: date 2016 01 01 23 59 59 or date\n");
    }
}
MSH_CMD_EXPORT(date, set date [year month day hour min sec]);
#endif /* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) */

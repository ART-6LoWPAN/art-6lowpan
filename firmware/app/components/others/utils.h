/*
 * utils.h
 *
 *  Created on: 2017Äê11ÔÂ1ÈÕ
 *      Author: armink
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <rtthread.h>

#if RT_TICK_PER_SECOND == 1
    #define DELAY_1S            (RT_TICK_PER_SECOND)
    #define DELAY_S(X)          (X*DELAY_1S)

#elif RT_TICK_PER_SECOND == 10
    #define DELAY_100MS(X)      (X)
    #define DELAY_S(X)          (X*10)

#elif RT_TICK_PER_SECOND == 100
    #define DELAY_10MS(X)       (X)
    #define DELAY_100MS(X)      (X*10)
    #define DELAY_S(X)          (X*100)

#elif (RT_TICK_PER_SECOND == 1000)

    #define DELAY_1MS           (RT_TICK_PER_SECOND/1000)
    #define DELAY_MS(X)         (X*DELAY_1MS)
    #define DELAY_S(X)          (X*1000*DELAY_1MS)

#elif (RT_TICK_PER_SECOND == 10000)
    #define DELAY_100US(X)      (X*RT_TICK_PER_SECOND/10000)
    #define DELAY_1MS           (RT_TICK_PER_SECOND/1000)
    #define DELAY_MS(X)         (X*DELAY_1MS)
    #define DELAY_S(X)          (X*1000*DELAY_1MS)
#endif

typedef enum{
    SYSTEM_STATUS_INIT,
    SYSTEM_STATUS_RUN,
    SYSTEM_STATUS_FAULT,
    SYSTEM_STATUS_SLEEP,
} system_status_t;

void set_system_status(system_status_t status);
system_status_t get_system_status(void);

#endif /* _UTILS_H_ */

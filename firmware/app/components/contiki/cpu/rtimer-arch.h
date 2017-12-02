/**
 * \file
 *         Header file for the universal rtimer code
 * \author
 *         Armink, <armink.ztl@gmail.com>
 */

#ifndef RTIMER_ARCH_H_
#define RTIMER_ARCH_H_

#include "sys/rtimer.h"

#define RTIMER_ARCH_SECOND (10000L)

void rtimer_arch_set(rtimer_clock_t t);

rtimer_clock_t rtimer_arch_now(void);

#endif /* RTIMER_ARCH_H_ */

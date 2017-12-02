/*
 * This file is part of the ART-6LoWPAN Radio Library.
 *
 * Copyright (c) 2017, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Implementation of the arch-specific rtimer functions by universal rtimer.
 * Created on: 2017-09-15
 */

#include "contiki.h"
#include "sys/energest.h"
#include "sys/rtimer.h"
#include <time.h>

struct ctimer schedule_timer;

static void _rtimer_run_next(void *param) {
    rtimer_run_next();
}
/*---------------------------------------------------------------------------*/
/**
 * \brief We don't need to do anything special here. The RTC is initialised
 * elsewhere
 */
void rtimer_arch_init(void)
{
    return;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Schedules an rtimer task to be triggered at time t
 * \param t The time when the task will need executed.
 *
 * \e t is an absolute time, in other words the task will be executed AT
 * time \e t, not IN \e t rtimer ticks.
 *
 * This function schedules a one-shot event with the AON RTC.
 *
 * This functions converts \e to a value suitable for the AON RTC.
 */
void rtimer_arch_schedule(rtimer_clock_t t) {
    rtimer_clock_t now = RTIMER_NOW();

    ctimer_stop(&schedule_timer);
    ctimer_set(&schedule_timer, (t - now) * CLOCK_CONF_SECOND / RTIMER_SECOND, _rtimer_run_next, NULL);
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Returns the current real-time clock time
 * \return The current rtimer time in ticks
 *
 * The value is read from the AON RTC counter and converted to a number of
 * rtimer ticks
 *
 */
rtimer_clock_t rtimer_arch_now(void) {
    return clock() * (RTIMER_ARCH_SECOND / 1000L);
}

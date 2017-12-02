#include <sys/clock.h>
#include <sys/cc.h>
#include <sys/etimer.h>
#include <sys/rtimer.h>
#include <rtthread.h>
#include <stdbool.h>
#include <time.h>

static volatile clock_time_t current_clock = 0;
static volatile unsigned long current_seconds = 0;
static unsigned int second_countdown = CLOCK_SECOND;

static void contiki_tick_thread_entry(void *param) {
    while (true) {
        current_clock++;
        if (etimer_pending() && etimer_next_expiration_time() <= current_clock) {
            etimer_request_poll();
            /* printf("%d,%d\n", clock_time(),etimer_next_expiration_time   ()); */

        }
        if (--second_countdown == 0) {
            current_seconds++;
            second_countdown = CLOCK_SECOND;
        }
        rt_thread_delay(rt_tick_from_millisecond(1000 / CLOCK_SECOND));
    }
}

void clock_init() {
    static rt_thread_t thread = NULL;
    if (!thread) {
        thread = rt_thread_create("contiki_tick", contiki_tick_thread_entry, NULL, 256, 2, 8);
        if (thread) {
            rt_thread_startup(thread);
        }
    }
}

clock_time_t clock_time(void) {
    return current_clock;
}

unsigned long clock_seconds(void) {
    return current_seconds;
}

void watchdog_periodic(void) {
    extern void IWDG_Feed(void);
    IWDG_Feed();
}

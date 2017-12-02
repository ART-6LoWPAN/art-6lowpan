/*
 * This file is part of the ART-6LoWPAN Radio Library.
 *
 * Copyright (c) 2017, Armink, <armink.ztl@gmail.com>
 * Copyright (c) 2017, xidongxu, <xi_dongxu@126.com>
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
 * Function: SI446x pin driver to RT-Thread based hardware platform.
 * Created on: 2017-09-15
 */

#include <stdbool.h>
#include "../cpu/radio/radio_hal.h"
#include <rtthread.h>
#include <board.h>
#include <drivers/pin.h>

/* STM32F405RGT6 64 pins */
#define SI446X_PIN_SDN        3 /* PC14 */
#define SI446X_PIN_IRQ        2 /* PC13 */
#define SI446X_PIN_ENT       96
#define SI446X_PIN_ENP       95
#if SI446X_TRANS_LED_ENABLE
#define SI446X_PIN_RX_LED    56 /* PB4 */
#define SI446X_PIN_TX_LED    57 /* PB5 */
#endif /* SI446X_TRANS_LED_ENABLE */
#if RAW_RADIO_TEST
#define SI446X_PIN_TEST_KEY  10 /* PC2 */
#endif /* RAW_RADIO_TEST */

void si446x_sdn_enabled(bool enabled) {
    if (enabled) {
        rt_pin_write(SI446X_PIN_SDN, PIN_LOW);
    } else {
        rt_pin_write(SI446X_PIN_SDN, PIN_HIGH);
    }
}

bool si446x_irq_pin_state(void) {
    return rt_pin_read(SI446X_PIN_IRQ);
}

#if RAW_RADIO_TEST
bool si446x_test_key_press(void) {
    return rt_pin_read(SI446X_PIN_TEST_KEY) == PIN_LOW ? true : false;
}
#endif /* RAW_RADIO_TEST */

void si446x_trans_led_ctrl(bool rx_on, bool tx_on) {
#if SI446X_TRANS_LED_ENABLE
    rt_pin_write(SI446X_PIN_RX_LED, rx_on);
    rt_pin_write(SI446X_PIN_TX_LED, tx_on);
#endif /* SI446X_TRANS_LED_ENABLE */
}

static void irq_hdr(void *args) {
    if (!si446x_irq_pin_state()) {
        radio_notice_irq();
    }
}

void si446x_for_mcu_init(void)
{
    rt_pin_mode(SI446X_PIN_IRQ, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(SI446X_PIN_SDN, PIN_MODE_OUTPUT);
    rt_pin_mode(SI446X_PIN_ENT, PIN_MODE_OUTPUT);
    rt_pin_mode(SI446X_PIN_ENP, PIN_MODE_OUTPUT);

#if SI446X_TRANS_LED_ENABLE
    rt_pin_mode(SI446X_PIN_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(SI446X_PIN_TX_LED, PIN_MODE_OUTPUT);
#endif /* SI446X_TRANS_LED_ENABLE */

#if RAW_RADIO_TEST
    rt_pin_mode(SI446X_PIN_TEST_KEY, PIN_MODE_INPUT_PULLUP);
#endif /* RAW_RADIO_TEST */

    rt_pin_write(SI446X_PIN_ENT, PIN_HIGH);
    rt_pin_write(SI446X_PIN_ENP, PIN_LOW);

    rt_pin_attach_irq(SI446X_PIN_IRQ, PIN_IRQ_MODE_FALLING, irq_hdr, NULL);
    rt_pin_irq_enable(SI446X_PIN_IRQ, true);
}

void si446x_pa_enabled(bool enabled) {
    if (enabled) {
        rt_pin_write(SI446X_PIN_ENP, PIN_HIGH);
    } else {
        rt_pin_write(SI446X_PIN_ENP, PIN_LOW);
    }
}

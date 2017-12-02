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
 * Function: Serial line driver port.
 * Created on: 2017-11-09
 */

#include <rtthread.h>
#include <dev/serial-line.h>

#include "serial-line-arch.h"

static rt_uint16_t odev_open_flag;
static rt_err_t (*odev_rx_ind)(rt_device_t dev, rt_size_t size) = NULL;
static rt_device_t serial_dev = NULL;

void serial_line_arch_init(void) {

}

void serial_line_arch_set_input(const char *serial_name) {
    RT_ASSERT(serial_name);

    serial_dev = rt_device_find(serial_name);
}

static rt_err_t getchar_rx_ind(rt_device_t dev, rt_size_t size) {
    uint8_t ch;
    rt_size_t i;

    for (i = 0; i < size; i++) {
        /* read a char */
        if (rt_device_read(dev, 0, &ch, 1)) {
            serial_line_input_byte(ch);
            /* echo */
            rt_device_write(dev, 0, &ch, 1);
            if (ch == SERIAL_LINE_END_CHAR && SERIAL_LINE_END_CHAR == '\r') {
                ch = '\n';
                rt_device_write(dev, 0, &ch, 1);
            }
        }
    }
    return RT_EOK;
}

void serial_line_arch_start(void) {
    if (serial_dev) {
        /* backup RX indicate and flag */
        odev_rx_ind = serial_dev->rx_indicate;
        odev_open_flag = serial_dev->open_flag;
        serial_dev->open_flag &= ~RT_DEVICE_FLAG_STREAM;
        if (odev_open_flag & RT_DEVICE_OFLAG_OPEN) {
            rt_device_close(serial_dev);
        }
        rt_device_set_rx_indicate(serial_dev, getchar_rx_ind);
        rt_device_open(serial_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    }
}

void serial_line_arch_stop(void) {
    if (serial_dev && odev_rx_ind) {
        rt_device_close(serial_dev);
        /* restore RX indicate */
        rt_device_set_rx_indicate(serial_dev, odev_rx_ind);
        if (odev_open_flag & RT_DEVICE_OFLAG_OPEN) {
            rt_device_open(serial_dev, odev_open_flag);
        }
        serial_dev = NULL;
        odev_rx_ind = NULL;
    }
}

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
 * Function: Arch-specific SLIP functions for the RT-Thread.
 * Created on: 2017-10-11
 */

#include "contiki-conf.h"
#include "dev/slip.h"
#include <rtthread.h>
#include <rtdevice.h>

#if CONTIKI_SLIP_RAIDO
#define SLIP_DEV_NAME                  "uart1"
#else
#define SLIP_DEV_NAME                  "uart2"
#endif

static rt_device_t serial_dev = NULL;

static rt_err_t serial_ind(rt_device_t dev, rt_size_t size) {
    char ch;
    if (size > 0) {
        for (rt_size_t i = 0; i < size; i++) {
            rt_device_read(serial_dev, 0, &ch, 1);
            slip_input_byte(ch);
        }
    }
    return RT_EOK;
}

/*---------------------------------------------------------------------------*/
/**
 * \brief Write a byte over SLIP
 * \param c the byte
 */
void slip_arch_writeb(unsigned char c)
{
  rt_device_write(serial_dev, 0, &c, 1);
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Initialise the arch-specific SLIP driver
 * \param ubr Ignored for the rtt
 */
void slip_arch_init(unsigned long ubr)
{
    struct serial_configure serial_cfg = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t open_result = RT_EOK;

    /* 查找串口设备 */
    serial_dev = rt_device_find(SLIP_DEV_NAME);
    /* 必须查找成功，否则软件断言提示开发者 */
    RT_ASSERT(serial_dev);
    /* 配置串口参数 */
    serial_cfg.baud_rate = ubr;
    rt_device_control(serial_dev, RT_DEVICE_CTRL_CONFIG, &serial_cfg);
    /* 打开串口设备，默认打开的串口发送方式为轮询方式，接收为中断方式  */
    open_result = rt_device_open(serial_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    /* 必须打开成功，否则软件断言提示开发者 */
    RT_ASSERT(open_result == RT_EOK);
    rt_device_set_rx_indicate(serial_dev, serial_ind);
}
/*---------------------------------------------------------------------------*/

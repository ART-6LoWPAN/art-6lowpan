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
 * Function: Contiki main function port.
 * Created on: 2017-09-10
 */

#define LOG_TAG                        "contiki_main"

#include <elog.h>
#include "../cpu/radio/radio_hal.h"
#include <contiki.h>
#include <sys/clock.h>
#include <sys/etimer.h>
#include <sys/node-id.h>
#include <dev/serial-line.h>
#include <contiki-net.h>
#include <serial-line-arch.h>
#include <apps/shell/serial-shell.h>
#include <apps/shell/shell.h>

#include <stdio.h>
#include <stdbool.h>
#include <rtthread.h>

static bool init_ok = false;
unsigned short node_id = 0;

//PROCESS(hello_process, "hello");
//AUTOSTART_PROCESSES(&hello_process);
//PROCESS_THREAD(hello_process, ev, data) {
//    PROCESS_BEGIN();
//    while (1) {
//        static struct etimer et;
//        etimer_set(&et, CLOCK_SECOND);
//        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
////        rt_kprintf("send-packet-successful !\r\n");
//    }
//    PROCESS_END();
//}

static void contiki_main_thread_entry(void *param) {
    while (true) {
        uint8_t r;
        do {
            r = process_run();
            rt_thread_delay(rt_tick_from_millisecond(RT_TICK_PER_SECOND / CLOCK_SECOND));
        } while (r > 0);
    }
}

static void set_rf_params(void) {
    uint16_t short_addr = 0;
    uint8_t ext_addr[8] = { 0 };
    char node_addr[3 * LINKADDR_SIZE + 1] = { 0 };
    radio_value_t val = 0;

    /* read STM32 UUID as extended address */
    ext_addr[0] = 0x02;
    ext_addr[1] = 0x02;
    ext_addr[2] = 0x02;
    ext_addr[3] = *(uint8_t*) (0x1FFF7A18 + 0);
    ext_addr[4] = *(uint8_t*) (0x1FFF7A14 + 3);
    ext_addr[5] = *(uint8_t*) (0x1FFF7A14 + 0);
    ext_addr[6] = *(uint8_t*) (0x1FFF7A10 + 2);
    ext_addr[7] = *(uint8_t*) (0x1FFF7A10 + 0);
    /* calculate short address */
    short_addr = (ext_addr[6] << 8) + ext_addr[7];

    NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, RF_CORE_CONF_CHANNEL);
    NETSTACK_RADIO.set_object(RADIO_PARAM_64BIT_ADDR, ext_addr, 8);
    /* Populate linkaddr_node_addr. Maintain endianness */
    memcpy(&linkaddr_node_addr, ext_addr, LINKADDR_SIZE);

    NETSTACK_RADIO.get_value(RADIO_PARAM_CHANNEL, &val);
    log_i(" RF: Channel %d", val);

#if STARTUP_CONF_VERBOSE
    {
        int i;
        for (i = 0; i < LINKADDR_SIZE - 1; i++) {
            snprintf(node_addr + i * 3, 3 + 1, "%02x:", linkaddr_node_addr.u8[i]);
        }
        snprintf(node_addr + i * 3, 3 + 1, "%02x", linkaddr_node_addr.u8[i]);
        log_i(" Link layer addr: %s", node_addr);
    }
#endif

    /* also set the global node id */
    node_id = short_addr;
    log_i(" Node ID: %d", node_id);
}

void contiki_init(void) {

    if (init_ok) {
        return;
    }

    extern void si446x_for_mcu_init(void);
    si446x_for_mcu_init();

    clock_init();

    rtimer_init();

    process_init();

    random_init(0x1234);

    serial_line_arch_init();

    log_i("Starting " CONTIKI_VERSION_STRING);

    process_start(&etimer_process, NULL);

    ctimer_init();

    serial_line_init();

    log_i(" Net: %s", NETSTACK_NETWORK.name);
    log_i(" MAC: %s", NETSTACK_MAC.name);
    log_i(" RDC: %s", NETSTACK_RDC.name);

    if(NETSTACK_RDC.channel_check_interval() != 0) {
        log_d(" Channel Check Interval: %u ticks", NETSTACK_RDC.channel_check_interval());
    }

    set_rf_params();

#if RAW_RADIO_TEST
    /* only init radio for RAW radio testing */
    NETSTACK_RADIO.init();
#else
    netstack_init();

#if NETSTACK_CONF_WITH_IPV6
    memcpy(&uip_lladdr.addr, &linkaddr_node_addr, sizeof(uip_lladdr.addr));
    queuebuf_init();
    process_start(&tcpip_process, NULL);
#endif /* NETSTACK_CONF_WITH_IPV6 */

    autostart_start(autostart_processes);
#endif /* RAW_RADIO_TEST */

#if COAP_ENABLE
	/* start coap */
#include <examples/er-rest-example/er-rest-example.h>
#if COAP_SERVER_ENABLE
	process_start(&er_example_server, NULL);
#else
	process_start(&er_example_client, NULL);
#endif /* COAP_SERVER_ENABLE */
#endif /* COAP_ENABLE */

    rt_thread_t thread = rt_thread_create("contiki_main", contiki_main_thread_entry, NULL, 2048, RT_THREAD_PRIORITY_MAX - 2, 9);
    if (thread) {
        rt_thread_startup(thread);
        init_ok = true;
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
static void contiki(uint8_t argc, char **argv) {
    serial_line_arch_set_input(RT_CONSOLE_DEVICE_NAME);

    serial_shell_init();

//    shell_memdebug_init();
    shell_netstat_init();
    shell_ping_init();
//    shell_power_init();
    shell_ps_init();
//    shell_run_init();
//    shell_text_init();
    shell_udpsend_init();
}
FINSH_FUNCTION_EXPORT(contiki, Open contiki shell);
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(contiki, Open contiki shell);
#endif /* FINSH_USING_MSH */
#endif /* RT_USING_FINSH */




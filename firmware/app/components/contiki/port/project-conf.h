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
 * Function: Contiki configuration for this project. Overrides the `contiki_conf.h` .
 * Created on: 2017-09-15
 */

/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
#define PRAGMA(x)                             _Pragma(#x)
#define CC_ALIGN(n)                           PRAGMA(data_alignment=n)
/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
#define IEEE802154_CONF_PANID                 0xABCD
#define RF_CORE_CONF_CHANNEL                  0
#define RF_CORE_CONF_DATARATE_20K             1
#define RF_CORE_CONF_DATARATE_10K             0
#define RF_CORE_CONF_DATARATE_4800            0
#define RF_CORE_CONF_DATARATE_2400            0
/*---------------------------------------------------------------------------*/
#define CONTIKI                               1
#define AUTOSTART_ENABLE                      1
/*---------------------------------------------------------------------------*/
/* IPv6  */
#define NETSTACK_CONF_WITH_IPV6               1
#if NETSTACK_CONF_WITH_IPV6
    #define UIP_CONF_ICMP6                    1
    #define UIP_CONF_IPV6_RPL                 1
    #define UIP_CONF_ROUTER                   1
    #define UIP_CONF_ND6_SEND_NA              1
    #define STARTUP_CONF_VERBOSE              1
    #define NETSTACK_CONF_RDC                 nullrdc_driver
    #define UIP_CONF_BROADCAST                1
#endif /* NETSTACK_CONF_WITH_IPV6 */
/*---------------------------------------------------------------------------*/
/* RPL */
#ifdef UIP_CONF_IPV6_RPL
    #define RPL_CONF_DIS_INTERVAL             5
    #define RPL_CONF_DEFAULT_LIFETIME_UNIT    5
#endif
/*---------------------------------------------------------------------------*/
/* MAC layer ACK */
#define MAC_ACK_WAIT(rtimer_clock)                         \
    do {                                                   \
        rt_thread_delay(rt_tick_from_millisecond(rtimer_clock/(RTIMER_ARCH_SECOND /1000)));        \
    } while(0)
/*---------------------------------------------------------------------------*/
/* Serial line */
#define SERIAL_LINE_CONF_BUFSIZE              512
#define SERIAL_LINE_IGNORE_CHAR(c)            (c == 0)
#define SERIAL_LINE_END_CHAR                  '\r'
/*---------------------------------------------------------------------------*/
/* UIP netstack & SLIP radio */
#define CONTIKI_SLIP_RAIDO                    0
#if CONTIKI_SLIP_RAIDO
    #define QUEUEBUF_CONF_NUM                 4
    #define UIP_CONF_BUFFER_SIZE              512
    #undef UIP_CONF_ROUTER
    #define UIP_CONF_ROUTER                   0
    #define CMD_CONF_OUTPUT                   slip_radio_cmd_output
    #define CMD_CONF_HANDLERS                 slip_radio_cmd_handler
    /* configuration for the slipradio/network driver */
    #undef NETSTACK_CONF_MAC
    #define NETSTACK_CONF_MAC                 nullmac_driver
    #undef NETSTACK_CONF_RDC
    #define NETSTACK_CONF_RDC                 nullrdc_driver
    #undef NETSTACK_CONF_NETWORK
    #define NETSTACK_CONF_NETWORK             slipnet_driver
    #undef NETSTACK_CONF_FRAMER
    #define NETSTACK_CONF_FRAMER              no_framer
#else
    #undef NETSTACK_CONF_MAC
    #define NETSTACK_CONF_MAC                 csma_driver
    #undef NETSTACK_CONF_RDC
    #define NETSTACK_CONF_RDC                 nullrdc_driver
    #define UIP_CONF_BUFFER_SIZE              900
    #define NBR_TABLE_CONF_MAX_NEIGHBORS      5
    #define UIP_CONF_MAX_ROUTES               5
    #define UIP_CONF_TCP_MSS                  128
#endif /* CONTIKI_SLIP_RAIDO */
/*---------------------------------------------------------------------------*/
/* CoAP */
#define COAP_ENABLE                           1
#define COAP_SERVER_ENABLE                    1
#if COAP_ENABLE
    #define REST                              REGISTERED_ENGINE_ERBIUM
#endif /* COAP_ENABLE */

#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/

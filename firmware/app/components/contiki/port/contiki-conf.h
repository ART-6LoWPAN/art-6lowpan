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
 * Function: Contiki configuration for the si446x platform.
 * Created on: 2017-09-15
 */

#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#include <stdint.h>
/*---------------------------------------------------------------------------*/
/* Include Project Specific conf */
#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */
/*---------------------------------------------------------------------------*/
/**
 * \name Network Stack Configuration
 *
 * @{
 */
#ifndef NETSTACK_CONF_NETWORK
#if NETSTACK_CONF_WITH_IPV6
#define NETSTACK_CONF_NETWORK sicslowpan_driver
#else
#define NETSTACK_CONF_NETWORK rime_driver
#endif /* NETSTACK_CONF_WITH_IPV6 */
#endif /* NETSTACK_CONF_NETWORK */

#ifndef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     csma_driver
#endif

#ifndef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     contikimac_driver
#endif

/* Configure NullRDC for when it's selected */
#define NULLRDC_CONF_802154_AUTOACK             1

/* Configure ContikiMAC for when it's selected */
#define CONTIKIMAC_CONF_WITH_CONTIKIMAC_HEADER  0
#define CONTIKIMAC_CONF_WITH_PHASE_OPTIMIZATION 0
#define WITH_FAST_SLEEP                         1

#ifndef NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE
#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE    8
#endif

#ifndef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154
#endif

#ifdef RF_CHANNEL
#define RF_CORE_CONF_CHANNEL             RF_CHANNEL
#endif

#define NETSTACK_CONF_RADIO                       si446x_driver

#ifndef RF_CORE_CONF_CHANNEL
#define RF_CORE_CONF_CHANNEL                      0
#endif

#if RF_CORE_CONF_DATARATE_20K
    #define NULLRDC_CONF_ACK_WAIT_TIME            (RTIMER_SECOND / 40)
#elif RF_CORE_CONF_DATARATE_10K
    #define NULLRDC_CONF_ACK_WAIT_TIME            (RTIMER_SECOND / 25)
#elif RF_CORE_CONF_DATARATE_4800
    #define NULLRDC_CONF_ACK_WAIT_TIME            (RTIMER_SECOND / 15)
#elif RF_CORE_CONF_DATARATE_2400
    #define NULLRDC_CONF_ACK_WAIT_TIME            (RTIMER_SECOND / 8)
#endif /* RF_CORE_CONF_DATARATE_20K */
#define NULLRDC_CONF_AFTER_ACK_DETECTED_WAIT_TIME (RTIMER_SECOND / 1000)
#define NULLRDC_CONF_802154_AUTOACK_HW            0
#define NULLRDC_CONF_SEND_802154_ACK              1

#define CONTIKIMAC_CONF_CCA_CHECK_TIME            (RTIMER_ARCH_SECOND / 1600)
#define CONTIKIMAC_CONF_CCA_SLEEP_TIME            (RTIMER_ARCH_SECOND / 210)
#define CONTIKIMAC_CONF_LISTEN_TIME_AFTER_PACKET_DETECTED  (RTIMER_ARCH_SECOND / 20)
#define CONTIKIMAC_CONF_SEND_SW_ACK               1
#define CONTIKIMAC_CONF_AFTER_ACK_DETECTED_WAIT_TIME (RTIMER_SECOND / 920)
#define CONTIKIMAC_CONF_INTER_PACKET_INTERVAL     (RTIMER_SECOND / 220)

#define NETSTACK_RADIO_MAX_PAYLOAD_LEN            125

/* 6LoWPAN */
#define SICSLOWPAN_CONF_COMPRESSION               SICSLOWPAN_COMPRESSION_HC06
#define SICSLOWPAN_CONF_COMPRESSION_THRESHOLD     63
#define SICSLOWPAN_CONF_FRAG                      1
#define SICSLOWPAN_CONF_MAXAGE                    8
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name IEEE address configuration
 *
 * Used to generate our RIME & IPv6 address
 * @{
 */
/**
 * \brief Location of the IEEE address
 * 0 => Read from InfoPage,
 * 1 => Use a hardcoded address, configured by IEEE_ADDR_CONF_ADDRESS
 */
#ifndef IEEE_ADDR_CONF_HARDCODED
#define IEEE_ADDR_CONF_HARDCODED             0
#endif

/**
 * \brief The hardcoded IEEE address to be used when IEEE_ADDR_CONF_HARDCODED
 * is defined as 1
 */
#ifndef IEEE_ADDR_CONF_ADDRESS
#define IEEE_ADDR_CONF_ADDRESS { 0x00, 0x12, 0x4B, 0x00, 0x89, 0xAB, 0xCD, 0xEF }
#endif
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name RF configuration
 *
 * @{
 */
/* RF Config */
#ifndef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID           0xABCD /**< Default PAN ID */
#endif

/** @} */
/*---------------------------------------------------------------------------*/
/** @} */
/**
 * \name IPv6, RIME and network buffer configuration
 *
 * @{
 */
/* Don't let contiki-default-conf.h decide if we are an IPv6 build */
#ifndef NETSTACK_CONF_WITH_IPV6
#define NETSTACK_CONF_WITH_IPV6              0
#endif

#if NETSTACK_CONF_WITH_IPV6
/*---------------------------------------------------------------------------*/
/* Addresses, Sizes and Interfaces */
#define LINKADDR_CONF_SIZE                   8
#define UIP_CONF_LL_802154                   1
#define UIP_CONF_LLH_LEN                     0

/* The size of the uIP main buffer */
#ifndef UIP_CONF_BUFFER_SIZE
#define UIP_CONF_BUFFER_SIZE              1000
#endif

/* ND and Routing */
#ifndef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER                      1
#endif

#define UIP_CONF_ND6_SEND_RA                 0
#define UIP_CONF_IP_FORWARD                  0
#define RPL_CONF_STATS                       0

#define UIP_CONF_ND6_REACHABLE_TIME     600000
#define UIP_CONF_ND6_RETRANS_TIMER       10000

#ifndef NBR_TABLE_CONF_MAX_NEIGHBORS
#define NBR_TABLE_CONF_MAX_NEIGHBORS        20
#endif
#ifndef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES                 20
#endif

#ifndef UIP_CONF_TCP
#define UIP_CONF_TCP                         1
#endif
#ifndef UIP_CONF_TCP_MSS
#define UIP_CONF_TCP_MSS                    64
#endif

#define UIP_CONF_UDP                         1
#define UIP_CONF_UDP_CHECKSUMS               1
#define UIP_CONF_ICMP6                       1
/*---------------------------------------------------------------------------*/
#else /* NETSTACK_CONF_WITH_IPV6 */
/* Network setup for non-IPv6 (rime). */
#define UIP_CONF_IP_FORWARD                  1

#define RIME_CONF_NO_POLITE_ANNOUCEMENTS     0

#endif /* NETSTACK_CONF_WITH_IPV6 */
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name Generic Configuration directives
 *
 * @{
 */
#ifndef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON            0 /**< Energest Module */
#endif
/** @} */
/*---------------------------------------------------------------------------*/
/**
 * \name Character I/O Configuration
 *
 * @{
 */

/* Turn off example-provided putchars */
#define SLIP_BRIDGE_CONF_NO_PUTCHAR        1
#define SLIP_RADIO_CONF_NO_PUTCHAR         1

#ifndef SLIP_ARCH_CONF_ENABLED
/*
 * Determine whether we need SLIP
 * This will keep working while UIP_FALLBACK_INTERFACE and CMD_CONF_OUTPUT
 * keep using SLIP
 */
#if defined(UIP_FALLBACK_INTERFACE) || defined(CMD_CONF_OUTPUT)
#define SLIP_ARCH_CONF_ENABLED             1
#endif
#endif
/*---------------------------------------------------------------------------*/
/**
 * \name Compiler configuration and platform-specific type definitions
 *
 * Those values are not meant to be modified by the user
 * @{
 */
#define CLOCK_CONF_SECOND 1000

/* Compiler configurations */
#define CCIF
#define CLIF

/* Platform typedefs */
typedef uint32_t clock_time_t;
typedef uint32_t uip_stats_t;

/* Clock (time) comparison macro */
#define CLOCK_LT(a, b)  ((signed long)((a) - (b)) < 0)

/*
 * rtimer.h typedefs rtimer_clock_t as unsigned short. We need to define
 * RTIMER_CLOCK_DIFF to override this
 */
typedef uint32_t rtimer_clock_t;
#define RTIMER_CLOCK_DIFF(a, b)     ((int32_t)((a) - (b)))

/* --------------------------------------------------------------------- */
/* TSCH related defines */

/* Delay between GO signal and SFD */
#define RADIO_DELAY_BEFORE_TX ((unsigned)US_TO_RTIMERTICKS(81))
/* Delay between GO signal and start listening.
 * This value is so small because the radio is constantly on within each timeslot. */
#define RADIO_DELAY_BEFORE_RX ((unsigned)US_TO_RTIMERTICKS(15))
/* Delay between the SFD finishes arriving and it is detected in software. */
#define RADIO_DELAY_BEFORE_DETECT ((unsigned)US_TO_RTIMERTICKS(352))

/* Timer conversion; radio is running at 4 MHz */
#define RADIO_TIMER_SECOND   4000000u
#if (RTIMER_SECOND % 256) || (RADIO_TIMER_SECOND % 256)
#error RADIO_TO_RTIMER macro must be fixed!
#endif
#define RADIO_TO_RTIMER(X)   ((uint32_t)(((uint64_t)(X) * (RTIMER_SECOND / 256)) / (RADIO_TIMER_SECOND / 256)))
#define USEC_TO_RADIO(X)     ((X) * 4)

/* The PHY header (preamble + SFD, 4+1 bytes) duration is equivalent to 10 symbols */
#define RADIO_IEEE_802154_PHY_HEADER_DURATION_USEC 160

/* Do not turn off TSCH within a timeslot: not enough time */
#define TSCH_CONF_RADIO_ON_DURING_TIMESLOT 1

/* Disable TSCH frame filtering */
#define TSCH_CONF_HW_FRAME_FILTERING	0

/* Use hardware timestamps */
#ifndef TSCH_CONF_RESYNC_WITH_SFD_TIMESTAMPS
#define TSCH_CONF_RESYNC_WITH_SFD_TIMESTAMPS 1
#define TSCH_CONF_TIMESYNC_REMOVE_JITTER 0
#endif

#ifndef TSCH_CONF_BASE_DRIFT_PPM
/* The drift compared to "true" 10ms slots.
 * Enable adaptive sync to enable compensation for this. */
#define TSCH_CONF_BASE_DRIFT_PPM -977
#endif

/* 10 times per second */
#ifndef TSCH_CONF_CHANNEL_SCAN_DURATION
#define TSCH_CONF_CHANNEL_SCAN_DURATION (CLOCK_SECOND / 10)
#endif

/* Slightly reduce the TSCH guard time (from 2200 usec to 1800 usec) to make sure
 * the CC26xx radio has sufficient time to start up. */
#ifndef TSCH_CONF_RX_WAIT
#define TSCH_CONF_RX_WAIT 1800
#endif

/** @} */
/*---------------------------------------------------------------------------*/
/* board.h assumes that basic configuration is done */
//#include "board.h"
/*---------------------------------------------------------------------------*/
#endif /* CONTIKI_CONF_H */

/** @} */

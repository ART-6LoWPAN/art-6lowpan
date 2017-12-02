/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Erbium (Er) CoAP client example.
 * \author
 *      Matthias Kovatsch <kovatsch@inf.ethz.ch>
 */
#define LOG_TAG    "coap_client"
#define LOG_LVL    ELOG_LVL_DEBUG

#include <elog.h>
#include "rtthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "er-coap-engine.h"

#include "net/rpl/rpl.h"
#include "net/ip/uip.h"
#include "string.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF("[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x]", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7], ((uint8_t *)addr)[8], ((uint8_t *)addr)[9], ((uint8_t *)addr)[10], ((uint8_t *)addr)[11], ((uint8_t *)addr)[12], ((uint8_t *)addr)[13], ((uint8_t *)addr)[14], ((uint8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF("[%02x:%02x:%02x:%02x:%02x:%02x]", (lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3], (lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif

/* FIXME: This server address is hard-coded for Cooja and link-local for unconnected border router. */
//#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xbbbb, 0, 0, 0, 0, 0, 0, 0x100)      /* cooja2 */
#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xbbbb, 0, 0, 0, 0x4664, 0x1B11, 0xB27B, 0x19ED)
#define LOCAL_PORT      UIP_HTONS(COAP_DEFAULT_PORT + 1)
#define REMOTE_PORT     UIP_HTONS(COAP_DEFAULT_PORT)

#define TOGGLE_INTERVAL 2

PROCESS(er_example_client, "Erbium Example Client");
//AUTOSTART_PROCESSES(&er_example_client);

uip_ipaddr_t server_ipaddr;
static struct etimer et;

/* Example URIs that can be queried. */
#define NUMBER_OF_URLS 4
/* leading and ending slashes only for demo purposes, get cropped automatically when setting the Uri-Path */
char *service_urls[NUMBER_OF_URLS] =
{ ".well-known/core", "/monitor", "battery/", "error/in//path" };

/* This function is will be passed to COAP_BLOCKING_REQUEST() to handle responses. */
void client_chunk_handler(void *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    log_d("coap recive：%.*s", len, (char * )chunk);
}

PROCESS_THREAD(er_example_client, ev, data) {

    PROCESS_BEGIN();
    static  char msg[] = {"[23.0,50.0,123.0,100.0,10.0,0.50]"};
    static coap_packet_t request[1]; /* This way the packet can be treated as pointer as usual. */

#ifdef auto_find_server
    /* 测试 */
    static uip_ip6addr_t *globaladdr = NULL;
    static rpl_dag_t *dag;
    uip_ds6_addr_t *addr_desc = uip_ds6_get_global(ADDR_PREFERRED);
    if(addr_desc != NULL) {
      globaladdr = &addr_desc->ipaddr;
      dag = rpl_get_any_dag();
      if(dag) {
          uip_ipaddr_copy(&server_ipaddr, globaladdr);
          memcpy(&server_ipaddr.u8[8], &dag->dag_id.u8[8], sizeof(uip_ipaddr_t) / 2);
      } else {
          SERVER_NODE(&server_ipaddr);
      }
    }
#else
    SERVER_NODE(&server_ipaddr);
#endif
    /* receives all CoAP messages */
    coap_init_engine();
    etimer_set(&et, TOGGLE_INTERVAL * CLOCK_SECOND);

    while (1) {
        PROCESS_YIELD();
        if (etimer_expired(&et)) {
            log_d("--Toggle timer--");
            /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, service_urls[1]);
            coap_set_payload(request, (uint8_t *) msg, sizeof(msg) - 1);
            PRINT6ADDR(&server_ipaddr);
            log_d(" : %u", UIP_HTONS(REMOTE_PORT));
            COAP_BLOCKING_REQUEST(&server_ipaddr, REMOTE_PORT, request, client_chunk_handler);
            log_d("--Done--");
            etimer_reset(&et);
        }
    }
    PROCESS_END();
}

//#include <stdio.h>
//#include <stdbool.h>
//#include <rtthread.h>
//#include <s2j.h>
//
//static void coap_client_thread_entry(void *param) {
//
//    static coap_packet_t request[1];
//    SERVER_NODE(&server_ipaddr);
//    /* receives all CoAP messages */
//    coap_init_engine();
//
//    /* 初始化数组 */
//    int payload[6] = {0,1,2,3,4,5};
//    /* 创建空JSON对象 */
//    s2j_create_json_obj(json_payload);
//    json_payload = cJSON_CreateIntArray(payload,sizeof(payload));
//    /* 获取数组成员 */
//    for (uint8_t i = 0; i < (sizeof(payload) / sizeof(payload[0])); i++) {
//        rt_kprintf("payload[%d] = %d \r\n", i, cJSON_GetArrayItem(json_payload, i)->valueint);
//    }
//    while (1) {
//        log_d("--Toggle timer--");
//        /* prepare request, TID is set by COAP_BLOCKING_REQUEST() */
//        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
//        coap_set_header_uri_path(request, service_urls[1]);
//        coap_set_payload(request, (uint8_t *)json_payload, sizeof(json_payload) - 1);
//        PRINT6ADDR(&server_ipaddr);
//        log_d(" : %u", UIP_HTONS(REMOTE_PORT));
//        coap_send_message(&server_ipaddr, REMOTE_PORT, (uint8_t *)request, sizeof(request));
//        log_d("--Done--");
//        rt_thread_delay(rt_tick_from_millisecond(1000));
//    }
//}
//
void coap_client_init(void) {
//    static uint8_t init_ok = false;
//    if(init_ok) {
//        return;
//    }
//    rt_thread_t thread = rt_thread_create("coap_client", coap_client_thread_entry, NULL, 1024, RT_THREAD_PRIORITY_MAX - 5, 10);
//    if (thread) {
//        rt_thread_startup(thread);
//        init_ok = true;
//    }
}


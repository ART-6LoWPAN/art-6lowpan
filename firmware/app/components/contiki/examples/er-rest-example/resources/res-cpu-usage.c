/*
 * This file is part of the ART-6LoWPAN.
 *
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
 * Function: Example of get the cpu usage resource.
 * Created on: 2017-12-08
 */

/**
 * \file
 *      Example of get the cpu usage resource
 */

#include "contiki.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include "rtthread.h"
#include <cpuusage.h>

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static uint8_t cpu_usage_major, cpu_usage_minor;

RESOURCE(res_cpu_usage,
         "title=\"CPU-usage\";rt=\"CPU-usage\";obs",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
    uint8_t length = 0;
    char usage[10] = { 0 }, usage_major[5] = { 0 }, usage_minor[5] = { 0 };
    cpu_usage_get(&cpu_usage_major, &cpu_usage_minor);
    snprintf(usage_major, sizeof(usage_major), "%d", cpu_usage_major);
    snprintf(usage_minor, sizeof(usage_minor), "%d", cpu_usage_minor);
    strncat(usage, usage_major, sizeof(usage_major));
    strncat(usage, ".", strlen("."));
    strncat(usage, usage_minor, sizeof(usage_minor));
    memcpy(buffer, usage, sizeof(usage));
    length = strlen((char *) buffer);
    REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
    REST.set_header_etag(response, (uint8_t *) &length, 1);
    REST.set_response_payload(response, (uint8_t *) buffer, length);
}

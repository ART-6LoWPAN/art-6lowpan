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
 * Function: Example of get the cpu inside temperature resource.
 * Created on: 2017-12-08
 */

/**
 * \file
 *      Example of get the cpu inside temperature resource
 */

#include "contiki.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include "../er-cpu-temperature.h"

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

RESOURCE(res_cpu_temperature,
        "title=\"CPU-temperature\";rt=\"CPU-temperature\";obs",
        res_get_handler,
        NULL,
        NULL,
        NULL);

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
    uint8_t length = 0;
    char temperature[10] = { 0 }, temperature_major[5] = { 0 };
    static uint8_t inited = 0;
    if (inited == 0) {
        get_cpu_temperature_init();
        inited = 1;
    }
    snprintf(temperature_major, sizeof(temperature_major), "%d", get_cpu_temperature());
    strncat(temperature, temperature_major, sizeof(temperature_major));
    memcpy(buffer, temperature, sizeof(temperature));
    length = strlen((char *) buffer);
    REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
    REST.set_header_etag(response, (uint8_t *) &length, 1);
    REST.set_response_payload(response, (uint8_t *) buffer, length);
}

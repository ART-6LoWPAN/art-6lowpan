/*
 * utils.c
 *
 *  Created on: 2017Äê11ÔÂ1ÈÕ
 *      Author: armink
 */

#include "utils.h"

/* current system status */
static system_status_t cur_status = SYSTEM_STATUS_INIT;


void set_system_status(system_status_t status) {
    cur_status = status;
}

system_status_t get_system_status(void) {
    return cur_status;
}

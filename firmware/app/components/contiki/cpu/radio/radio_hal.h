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
 * Function: Radio HAL(Hardware Abstraction Layer) layer driver. It provide radio API to `radio_trans_soft.c` or `radio_driver.c` .
 * Created on: 2017-09-15
 */

#ifndef _RADIO_HAL_H_
#define _RADIO_HAL_H_

#include "radio_comm.h"
#include "Si446x/si446x_prop.h"

#include <stdint.h>

/*! Maximal packet length definition (FIFO size) */
#define RADIO_MAX_PACKET_LENGTH         64u
/* 定义射频芯片的最大通道数 */
#define RADIO_MAX_CHANNEL_NUMBER        25
/* 报文处理器中，字段1（Filed1）长度 */
#define RADIO_PH_FILE1_LEN              2

typedef struct
{
    U8    *Radio_ConfigurationArray;

    U8   Radio_ChannelNumber;
    U8   Radio_PacketLength;
    U8   Radio_State_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;

    U8   Radio_CustomPayload[RADIO_MAX_PACKET_LENGTH];

} tRadioConfiguration;

void radio_hal_AssertShutdown(void);
void radio_hal_DeassertShutdown(void);
void radio_hal_ClearNsel(void);
void radio_hal_SetNsel(void);
BIT radio_hal_NirqLevel(void);

uint8_t radio_hal_init(void);
void radio_software_init(void);
void radio_hal_SpiWriteThenRead(U8 byteSend, U8* pSendData, U8 byteRecv, U8* pRecvData);
void radio_hal_SpiWriteThenWrite(U8 sendByte1, U8* pSendData1, U8 sendByte2, U8* pSendData2);
void radio_hal_SpiWriteData(U8 byteCount, U8* pData);
void radio_hal_SpiReadData(U8 byteCount, U8* pData);

U8 radio_find_property(U8* pbArray, U8 bGroup, U8 bAddress, U8* pbValue);
void radio_start_tx_variable_packet_multifield(uint8_t channel, uint8_t *radio_packet, uint8_t length);
void radio_start_tx_variable_packet(uint8_t channel, uint8_t *radio_packet, uint8_t length);
void radio_start_rx(uint8_t channel, uint8_t packet_lenght);

void radio_transmit_software_init(void);
void radio_notice_irq(void);
int radio_get_cur_rssi(void);
int radio_get_latch_rssi(void);
uint8_t radio_get_channel(void);
void radio_set_channel(uint8_t channel);
void radio_cmd_reply_lock(void);
void radio_cmd_reply_unlock(void);

#ifdef DRIVERS_EXTENDED_SUPPORT
  BIT radio_hal_Gpio0Level(void);
  BIT radio_hal_Gpio1Level(void);
  BIT radio_hal_Gpio2Level(void);
  BIT radio_hal_Gpio3Level(void);
#endif

#endif //_RADIO_HAL_H_

/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-01     armink      add board.h to this bsp
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <stm32f4xx.h>

// <o> Internal SRAM memory size[Kbytes] <8-128>
//  <i>Default: 128
#define STM32_SRAM_BEGIN        SRAM_BASE
#define STM32_SRAM_SIZE         128
#define STM32_SRAM_END          (SRAM_BASE + STM32_SRAM_SIZE * 1024)

#define VECT_TAB_USER           1
#if VECT_TAB_USER
/* user application start address */
#define USER_APP_ENTRY          (FLASH_BASE + 64 * 1024)
/* application vector table address */
#define USER_VECTOR_OFFSET      (USER_APP_ENTRY - NVIC_VectTab_FLASH)
#endif /* VECT_TAB_USER */

/* RT_USING_UART */
#define RT_USING_UART1
#define RT_USING_UART2
#define RT_USING_UART3

/* RT_USING_SPI */
#define RT_USING_SPI1
#define RT_USING_SPI2

/* Flash 默认分区表 */
#define RT_PARTITION_DEFAULT_TABLE               \
{                                                \
    {                                            \
        /* EasyFlash Backup Section */           \
        .name       = "ef_bak",                  \
        .offset     = 0,                         \
        .size       = 2 * 1024 * 1024,           \
        .flags = PARTITION_WRITEABLE,            \
    }, {                                         \
        /* Fatfs*/                               \
        .name       = "fatfs",                   \
        .offset     = 2 * 1024 * 1024,           \
        .size       = 14 * 1024 * 1024,          \
        .flags = PARTITION_WRITEABLE,            \
    }                                            \
}

#define HARDWARE_VERSION       "1.2"

#define SYSTEM_RUN_LED_ON                    GPIO_SetBits  (GPIOB,GPIO_Pin_3)                     //LED1亮
#define SYSTEM_RUN_LED_OFF                   GPIO_ResetBits(GPIOB,GPIO_Pin_3)                     //LED1灭

void rt_hw_board_init(void);
void IWDG_Feed(void);

#endif

/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-11-01     armink      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"

static void RCC_Configuration(void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);
    /* 确认晶振完全起振 */
    assert_param(rcc_clocks.HCLK_Frequency == 168000000);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | \
                           RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF | \
                           RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void NVIC_Configuration(void)
{
#if VECT_TAB_USER
    /* Set the Vector Table base location by user application firmware definition */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, USER_VECTOR_OFFSET);
#else
#ifdef VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif /* VECT_TAB_RAM */
#endif /* VECT_TAB_USER */
}

static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* JTAG-DP Disabled and SW-DP Enabled */
    /**
     * 使能SWD、失能JTAG功能，参考寄存器手册“8.3.2  I/O pin multiplexer and mapping”中的
     * <GPIO>小节，直接配置成普通GPIO即可
     */

    /* PA13:SWDIO PA14:SWCLK for debug, can't set to output mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All & (~(GPIO_Pin_13 | GPIO_Pin_14));
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

static void IWDG_Configuration(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64);
    IWDG_SetReload(1875);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}

void assert_failed(u8* file, u32 line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    rt_kprintf("assert failed at %s:%d \n", file, line);
    while (1) {
    }
}

/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    rt_uint32_t         cnts;

    RCC_GetClocksFreq(&rcc_clocks);

    cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;

    SysTick_Config(cnts);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
    RCC_Configuration();

    NVIC_Configuration();

    GPIO_Configuration();

#ifdef RELEASE
    IWDG_Configuration();
#endif /* RELEASE */

    /* Configure the SysTick */
    SysTick_Configuration();

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
}

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv) {
    NVIC_SystemReset();
}
FINSH_FUNCTION_EXPORT(reboot, reboot board);
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(reboot, Reboot System);
#endif /* FINSH_USING_MSH */
#endif /* RT_USING_FINSH */

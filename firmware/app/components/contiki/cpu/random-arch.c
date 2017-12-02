/*
 * This file is part of the ART-6LoWPAN Radio Library.
 *
 * Copyright (c) 2017, Armink, <armink.ztl@gmail.com>
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
 * Function: This file overrides core/lib/random.c and calls MCU-specific RNG functions.
 * Created on: 2017-09-15
 */

/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include "stm32f4xx_conf.h"
/*---------------------------------------------------------------------------*/
/**
 * \brief      Generates a new random number using the hardware TRNG.
 * \return     The random number.
 */
unsigned short random_rand(void)
{
    return (unsigned short) RNG_GetRandomNumber() & 0xFFFF;
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Function required by the API
 * \param seed Ignored.
 */
void random_init(unsigned short seed)
{
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);

    RNG_Cmd(ENABLE);

    while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);
}
/*---------------------------------------------------------------------------*/

/** \file compiler_defs.h
 *
 * \brief Register/bit definitions for cross-compiler projects on the C8051F93x/2x family.
 *
 * \b COPYRIGHT
 * \n Portions of this file are copyright Maarten Brock
 * \n http://sdcc.sourceforge.net
 * \n Portions of this file are copyright 2010, Silicon Laboratories, Inc.
 * \n http://www.silabs.com
 *
 *
 * <b> GNU LGPL boilerplate: </b>
 * \n This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * \n This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 * \n You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 * \n In other words, you are welcome to use, share and improve this program.
 * You are forbidden to forbid anyone else to use, share and improve
 * what you give them. Help stamp out software-hoarding!
 *
 * <b> Program Description: </b>
 * \note This header file should be included before including
 * a device-specific header file such as C8051F300_defs.h.
 *
 * Macro definitions to accommodate 8051 compiler differences in specifying
 * special function registers and other 8051-specific features such as NOP
 * generation, and locating variables in memory-specific segments.  The
 * compilers are identified by their unique predefined macros. See also:
 * http://predef.sourceforge.net/precomp.html
 *
 * SBIT and SFR define special bit and special function registers at the given
 * address. SFR16 and SFR32 define sfr combinations at adjacent addresses in
 * little-endian format. SFR16E and SFR32E define sfr combinations without
 * prerequisite byte order or adjacency. None of these multi-byte sfr
 * combinations will guarantee the order in which they are accessed when read
 * or written.
 *
 * SFR16X and SFR32X for 16 bit and 32 bit xdata registers are not defined
 * to avoid portability issues because of compiler endianness.
 *
 * Example:
 * \code
 * // my_mcu.c: main 'c' file for my mcu
 * #include <compiler_defs.h>  // this file
 * #include <C8051xxxx_defs.h> // SFR definitions for specific MCU target
 *
 * SBIT  (P0_1, 0x80, 1);      // Port 0 pin 1
 * SFR   (P0, 0x80);           // Port 0
 * SFRX  (CPUCS, 0xE600);      // Cypress FX2 Control and Status register in
 *                             // xdata memory at 0xE600
 * SFR16 (TMR2, 0xCC);         // Timer 2, lsb at 0xCC, msb at 0xCD
 * SFR16E(TMR0, 0x8C8A);       // Timer 0, lsb at 0x8A, msb at 0x8C
 * SFR32 (MAC0ACC, 0x93);      // SiLabs C8051F120 32 bits MAC0 Accumulator,
 *                             // lsb at 0x93, msb at 0x96
 * SFR32E(SUMR, 0xE5E4E3E2);   // TI MSC1210 SUMR 32 bits Summation register,
 *                             // lsb at 0xE2, msb at 0xE5
 * \endcode
 *
 * <b>Target:</b>         C8051xxxx
 * \n <b>Tool chain:</b>     Generic
 * \n <b>Command Line:</b>   None
 *
 * Release 2.3 - 27 MAY 2010 (DM)
 * \n    -Removed 'LOCATED_VARIABLE' pragma from Keil because it is not supported
 * \n Release 2.2 - 06 APR 2010 (ES)
 * \n    -Removed 'PATHINCLUDE' pragma from Raisonance section
 * \n Release 2.1 - 16 JUL 2009 (ES)
 * \n    -Added SEGMENT_POINTER macro definitions for SDCC, Keil, and Raisonance
 * \n    -Added LOCATED_VARIABLE_NO_INIT macro definitions for Raisonance
 * \n Release 2.0 - 19 MAY 2009 (ES)
 * \n    -Added LOCATED_VARIABLE_NO_INIT macro definitions for SDCC and Keil
 * \n Release 1.9 - 23 OCT 2008 (ES)
 * \n    -Updated Hi-Tech INTERRUPT and INTERRUPT_USING macro definitions
 * \n    -Added SFR16 macro defintion for Hi-Tech
 * \n Release 1.8 - 31 JUL 2008 (ES)
 * \n    -Added INTERRUPT_USING and FUNCTION_USING macro's
 * \n    -Added macro's for IAR
 * \n    -Corrected Union definitions for Hi-Tech and added SFR16 macro defintion
 * \n Release 1.7 - 11 SEP 2007 (BW)
 * \n    -Added support for Raisonance EVAL 03.03.42 and Tasking Eval 7.2r1
 * \n Release 1.6 - 27 AUG 2007 (BW)
 * \n    -Updated copyright notice per agreement with Maartin Brock
 * \n    -Added SDCC 2.7.0 "compiler.h" bug fixes
 * \n    -Added memory segment defines (SEG_XDATA, for example)
 * \n Release 1.5 - 24 AUG 2007 (BW)
 * \n    -Added support for NOP () macro
 * \n    -Added support for Hi-Tech ver 9.01
 * \n Release 1.4 - 07 AUG 2007 (PKC)
 * \n    -Removed FID and fixed formatting.
 * \n Release 1.3 - 30 SEP 2007 (TP)
 * \n    -Added INTERRUPT_PROTO_USING to properly support ISR context switching
 *      under SDCC.
 * \n Release 1.2 - (BW)
 * \n    -Added support for U8,U16,U32,S8,S16,S32,UU16,UU32 data types
 * \n Release 1.1 - (BW)
 * \n    -Added support for INTERRUPT, INTERRUPT_USING, INTERRUPT_PROTO,
 *      SEGMENT_VARIABLE, VARIABLE_SEGMENT_POINTER,
 *      SEGMENT_VARIABLE_SEGMENT_POINTER, and LOCATED_VARIABLE
 * \n Release 1.0 - 29 SEP 2006 (PKC)
 * \n    -Initial revision
 */

//-----------------------------------------------------------------------------
// Header File Preprocessor Directive
//-----------------------------------------------------------------------------

#ifndef COMPILER_DEFS_H
#define COMPILER_DEFS_H

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// SDCC - Small Device C Compiler
// http://sdcc.sourceforge.net

#if defined SDCC

# define SEG_GENERIC
# define SEG_FAR   __xdata
# define SEG_DATA  __data
# define SEG_NEAR  __data
# define SEG_IDATA __idata
# define SEG_XDATA __xdata
# define SEG_PDATA __pdata
# define SEG_CODE  __code
//# define SEG_BDATA __bdata

# define SBIT(name, addr, bit)  __sbit  __at(addr+bit)                  name
# define SFR(name, addr)        __sfr   __at(addr)                      name
# define SFRX(name, addr)       xdata volatile unsigned char __at(addr) name
# define SFR16(name, addr)      __sfr16 __at(((addr+1U)<<8) | addr)     name
# define SFR16E(name, fulladdr) __sfr16 __at(fulladdr)                  name
# define SFR32(name, addr)      __sfr32 __at(((addr+3UL)<<24) | ((addr+2UL)<<16) | ((addr+1UL)<<8) | addr) name
# define SFR32E(name, fulladdr) __sfr32 __at(fulladdr)                  name

#if (SDCC < 300)
# define INTERRUPT(name, vector) void name (void) interrupt (vector)
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt (vector) using (regnum)
# define INTERRUPT_PROTO(name, vector) void name (void) interrupt (vector)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void) interrupt (vector) using (regnum)
# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using (regnum)
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter) using (regnum)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)
# define SEGMENT_VARIABLE(name, vartype, locsegment) locsegment vartype name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) targsegment vartype * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) targsegment vartype * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment at (addr) vartype name = init
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) locsegment at (addr) vartype name
# define LOCATED_VARIABLE_POST_INIT(name, vartype, locsegment, addr) locsegment at (addr) vartype name
#else //SDCC >= 300
# define INTERRUPT(name, vector) void name (void) __interrupt (vector)
# define INTERRUPT_USING(name, vector, regnum) void name (void) __interrupt (vector) using (regnum)
# define INTERRUPT_PROTO(name, vector) void name (void) __interrupt (vector)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void) __interrupt (vector) using (regnum)
# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using (regnum)
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter) using (regnum)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)
# define SEGMENT_VARIABLE(name, vartype, locsegment) locsegment vartype name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) targsegment vartype * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) targsegment vartype * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment __at (addr) vartype name = init
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) locsegment __at (addr) vartype name
# define LOCATED_VARIABLE_POST_INIT(name, vartype, locsegment, addr) locsegment __at (addr) vartype name
#endif //SDCC >= 300

// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3

#if (SDCC < 300)
typedef bit BIT;
#else //SDCC >= 300
typedef __bit BIT;
#endif //SDCC >= 300

#define BITS(bitArray, bitPos)  BIT bitArray ## bitPos
#define WRITE_TO_BIT_ARRAY(bitArray, byte)  bitArray ## 0 = byte & 0x01; \
                                            bitArray ## 1 = byte & 0x02; \
                                            bitArray ## 2 = byte & 0x04; \
                                            bitArray ## 3 = byte & 0x08; \
                                            bitArray ## 4 = byte & 0x10; \
                                            bitArray ## 5 = byte & 0x20; \
                                            bitArray ## 6 = byte & 0x40; \
                                            bitArray ## 7 = byte & 0x80;

#define READ_FROM_BIT_ARRAY(bitArray, byte) byte =  (bitArray ## 0) | \
                                                   ((bitArray ## 1) << 1) | \
                                                   ((bitArray ## 2) << 2) | \
                                                   ((bitArray ## 3) << 3) | \
                                                   ((bitArray ## 4) << 4) | \
                                                   ((bitArray ## 5) << 5) | \
                                                   ((bitArray ## 6) << 6) | \
                                                   ((bitArray ## 7) << 7);

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

// NOP () macro support
#if (SDCC<300)
#define NOP() _asm NOP _endasm
#else //SDCC>= 300
#define NOP() __asm NOP __endasm
#endif //SDCC>= 300

//-----------------------------------------------------------------------------

// Raisonance (must be placed before Keil C51)
// http://www.raisonance.com

#elif defined __RC51__

//#error Raisonance C51 detected.

# define SEG_GENERIC generic     //SEG_GENERIC only applies to pointers in Raisonance, not variables.
# define SEG_FAR   xdata
# define SEG_DATA  data
# define SEG_NEAR  data
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata

# define SBIT(name, addr, bit)  at (addr+bit) sbit         name
# define SFR(name, addr)        sfr at addr                name
# define SFR16(name, addr)      sfr16 at addr              name
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) void name (void) interrupt vector
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using regnum
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) at addr locsegment vartype name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) at addr locsegment vartype name


// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef bit BIT;

#define BITS(bitArray, bitPos)  BIT bitArray ## bitPos
#define WRITE_TO_BIT_ARRAY(bitArray, byte)  bitArray ## 0 = byte & 0x01; \
                                            bitArray ## 1 = byte & 0x02; \
                                            bitArray ## 2 = byte & 0x04; \
                                            bitArray ## 3 = byte & 0x08; \
                                            bitArray ## 4 = byte & 0x10; \
                                            bitArray ## 5 = byte & 0x20; \
                                            bitArray ## 6 = byte & 0x40; \
                                            bitArray ## 7 = byte & 0x80;

#define READ_FROM_BIT_ARRAY(bitArray, byte) byte =  (bitArray ## 0) | \
                                                   ((bitArray ## 1) << 1) | \
                                                   ((bitArray ## 2) << 2) | \
                                                   ((bitArray ## 3) << 3) | \
                                                   ((bitArray ## 4) << 4) | \
                                                   ((bitArray ## 5) << 5) | \
                                                   ((bitArray ## 6) << 6) | \
                                                   ((bitArray ## 7) << 7);

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

// NOP () macro support -- NOP is opcode 0x00
#define NOP() asm { 0x00 }


//-----------------------------------------------------------------------------


// Keil C51
// http://www.keil.com

#elif (defined __C51__) || (defined __CX51__)

//#error Keil C51 detected.

# define SEG_GENERIC
# define SEG_FAR   xdata
# define SEG_DATA  data
# define SEG_NEAR  data
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata

# define SBIT(name, addr, bit)  sbit  name = addr^bit
# define SFR(name, addr)        sfr   name = addr
# define SFR16(name, addr)      sfr16 name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) void name (void) interrupt vector
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using regnum
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) vartype locsegment name _at_ addr

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef bit BIT;

#define BITS(bitArray, bitPos)  BIT bitArray ## bitPos
#define WRITE_TO_BIT_ARRAY(bitArray, byte)  bitArray ## 0 = byte & 0x01; \
                                            bitArray ## 1 = byte & 0x02; \
                                            bitArray ## 2 = byte & 0x04; \
                                            bitArray ## 3 = byte & 0x08; \
                                            bitArray ## 4 = byte & 0x10; \
                                            bitArray ## 5 = byte & 0x20; \
                                            bitArray ## 6 = byte & 0x40; \
                                            bitArray ## 7 = byte & 0x80;

#define READ_FROM_BIT_ARRAY(bitArray, byte) byte =  (bitArray ## 0) | \
                                                   ((bitArray ## 1) << 1) | \
                                                   ((bitArray ## 2) << 2) | \
                                                   ((bitArray ## 3) << 3) | \
                                                   ((bitArray ## 4) << 4) | \
                                                   ((bitArray ## 5) << 5) | \
                                                   ((bitArray ## 6) << 6) | \
                                                   ((bitArray ## 7) << 7);

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop_ (void);
#define NOP() _nop_()

//-----------------------------------------------------------------------------

// Hi-Tech 8051
// http://www.htsoft.com

#elif defined HI_TECH_C

# define SEG_GENERIC
# define SEG_FAR   far
# define SEG_DATA  data
# define SEG_NEAR  near
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata


# define SBIT(name, addr, thebit) static volatile bit name @ (addr + thebit)
# define SFR(name, addr)          static volatile unsigned char name @ addr
# define SFR16(name, addr)        static volatile unsigned int name @ addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector)       void name (void) interrupt vector
# define INTERRUPT_PROTO(name, vector)
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO_USING(name, vector, regnum)

# define FUNCTION_USING(name, return_value, parameter, regnum) /* not supported */
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) /* not supported */
// Note: Hi-Tech does not support functions using different register banks. Register
//       banks can only be specified in interrupts. If a function is called from
//       inside an interrupt, it will use the same register bank as the interrupt.

# define SEGMENT_VARIABLE(name, vartype, locsegment) locsegment vartype name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) targsegment vartype * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) targsegment vartype * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment vartype name @ addr

// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

// NOP () macro support
#define NOP() asm(" nop ")

//-----------------------------------------------------------------------------

// Tasking / Altium
// http://www.altium.com/tasking


#elif defined _CC51

# define SEG_GENERIC
# define SEG_FAR   _xdat
# define SEG_DATA  _data
# define SEG_NEAR  _data
# define SEG_IDATA _idat
# define SEG_XDATA _xdat
# define SEG_PDATA _pdat
# define SEG_CODE  _rom
# define SEG_BDATA _bdat

# define SBIT(name, addr, bit)  _sfrbit  name _at(addr+bit)
# define SFR(name, addr)        _sfrbyte name _at(addr)
# define SFRX(name, addr)       _xdat volatile unsigned char name _at(addr)
#if _CC51 > 71
# define SFR16(name, addr)      _sfrword _little name _at(addr)
#else
# define SFR16(name, addr)      /* not supported */
#endif
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) _interrupt (vector) void name (void)
# define INTERRUPT_USING(name, vector, regnum) _interrupt (vector) _using(regnum) void name (void)
# define INTERRUPT_PROTO(name, vector) _interrupt (vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) _interrupt (vector) _using(regnum) void name (void)

// When calling FUNCTION_USING in Tasking, the function must be called from an interrupt or Main which
// is also using the same register bank. If not, the compiler will generate an error.
# define FUNCTION_USING(name, return_value, parameter, regnum) _using(regnum) return_value name (parameter)
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) _using(regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) vartype locsegment name _at( addr )

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop (void);
#define NOP() _nop()

//-----------------------------------------------------------------------------


// IAR 8051
// http://www.iar.com

#elif defined __ICC8051__

#include <stdbool.h>
#include <intrinsics.h>

# define SBIT(name, addr, bit)  __bit __no_init volatile bool name @ (addr+bit)
# define SFR(name, addr)        __sfr __no_init volatile unsigned char name @ addr
# define SFRX(name, addr)       __xdata __no_init volatile unsigned char name @ addr
# define SFR16(name, addr)      __sfr __no_init volatile unsigned int  name @ addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr) /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define SEG_GENERIC __generic
# define SEG_FAR  __xdata
# define SEG_DATA __data
# define SEG_NEAR __data
# define SEG_IDATA __idata
# define SEG_XDATA __xdata
# define SEG_PDATA __pdata
# define SEG_CODE  __code
# define SEG_BDATA __bdata

#define bit bool

# define _PPTOSTR_(x) #x
# define _PPARAM_(address) _PPTOSTR_(vector=address * 8 + 3)
# define _PPARAM2_(regbank) _PPTOSTR_(register_bank=regbank)
# define INTERRUPT(name, vector) _Pragma(_PPARAM_(vector)) __interrupt void name(void)
# define INTERRUPT_PROTO(name, vector)  __interrupt void name(void)
# define INTERRUPT_USING(name, vector, regnum) _Pragma(_PPARAM2_(regnum)) _Pragma(_PPARAM_(vector)) __interrupt void name(void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) __interrupt void name(void)

# define FUNCTION_USING(name, return_value, parameter, regnum) /* not supported */
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) /* not supported */
// Note: IAR does not support functions using different register banks. Register
//       banks can only be specified in interrupts. If a function is called from
//       inside an interrupt, it will use the same register bank as the interrupt.

# define SEGMENT_VARIABLE(name, vartype, locsegment)  locsegment vartype name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment  * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name

# define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment __no_init vartype name @ addr

// used with UU16
# define LSB 0
# define MSB 1

// used with UU32 (b0 is least-significant byte)
# define b0 0
# define b1 1
# define b2 2
# define b3 3

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;


#define NOP() __no_operation();
//-----------------------------------------------------------------------------

// Crossware
// http://www.crossware.com

#elif defined _XC51_VER
# define SBIT(name, addr, bit)  _sfrbit  name = (addr+bit)
# define SFR(name, addr)        _sfr     name = addr
# define SFR16(name, addr)      _sfrword name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

//-----------------------------------------------------------------------------

// Wickenhäuser
// http://www.wickenhaeuser.de

#elif defined __UC__
# define SBIT(name, addr, bit)  unsigned char bit  name @ (addr+bit)
# define SFR(name, addr)        near unsigned char name @ addr
# define SFR16(name, addr)      /* not supported */
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

//-----------------------------------------------------------------------------

// Default
// Unknown compiler

#else
//# warning unrecognized compiler

#define SEG_FAR
#define SEG_DATA
#define SEG_NEAR
#define SEG_IDATA
#define SEG_XDATA
#define SEG_PDATA
#define SEG_CODE
#define SEG_BDATA

#define SBIT(name, addr, bit)  volatile unsigned char  name
#define SFR(name, addr)        volatile unsigned char  name
#define SFRX(name, addr)       volatile unsigned char  name
#define SFR16(name, addr)      volatile unsigned short name
#define SFR16E(name, fulladdr) volatile unsigned short name
#define SFR32(name, fulladdr)  volatile unsigned long  name
#define SFR32E(name, fulladdr) volatile unsigned long  name
#define INTERRUPT(name, vector) void name (void)
#define INTERRUPT_USING(name, vector, regnum) void name (void)
#define INTERRUPT_PROTO(name, vector) void name (void)
#define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)
#define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter)
#define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

#define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
#define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
#define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
#define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
#define LOCATED_VARIABLE(name, vartype, locsegment, addr, init) locsegment vartype name
#define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) locsegment vartype name


// used with UU16
#define LSB 0
#define MSB 1

// used with UU32 (b0 is least-significant byte)
#define b0 0
#define b1 1
#define b2 2
#define b3 3

typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32 U32;
    S32 S32;
    UU16 UU16[2];
    U16 U16[2];
    S16 S16[2];
    U8 U8[4];
    S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop (void);
#define NOP() _nop()

typedef unsigned char bit;
typedef bit BIT;


#define BITS(bitArray, bitPos)  BIT bitArray ## bitPos
#define WRITE_TO_BIT_ARRAY(bitArray, byte)  bitArray ## 0 = byte & 0x01; \
                                            bitArray ## 1 = byte & 0x02; \
                                            bitArray ## 2 = byte & 0x04; \
                                            bitArray ## 3 = byte & 0x08; \
                                            bitArray ## 4 = byte & 0x10; \
                                            bitArray ## 5 = byte & 0x20; \
                                            bitArray ## 6 = byte & 0x40; \
                                            bitArray ## 7 = byte & 0x80;

#define READ_FROM_BIT_ARRAY(bitArray, byte) byte =  (bitArray ## 0) | \
                                                   ((bitArray ## 1) << 1) | \
                                                   ((bitArray ## 2) << 2) | \
                                                   ((bitArray ## 3) << 3) | \
                                                   ((bitArray ## 4) << 4) | \
                                                   ((bitArray ## 5) << 5) | \
                                                   ((bitArray ## 6) << 6) | \
                                                   ((bitArray ## 7) << 7);


#endif

//-----------------------------------------------------------------------------
// Compiler independent data type definitions
//-----------------------------------------------------------------------------
#ifndef   FALSE
#define   FALSE     0
#endif
#ifndef   TRUE
#define   TRUE      !FALSE
#endif

#ifndef   NULL
#define   NULL      ((void *) 0)
#endif

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------

#endif                                 // #define COMPILER_DEFS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

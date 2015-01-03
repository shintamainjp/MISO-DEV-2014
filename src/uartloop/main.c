/**
 * @file main.c
 * @author Shinichiro Nakamura
 * @copyright
 * ===============================================================
 * MISO-DEV-2014
 * ===============================================================
 * Copyright (c) 2014-2015 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * ===============================================================
 */

#include <cdefBF512.h>
#include <builtins.h>
#include "cpu.h"
// #include "mruby.h"

static void sdram_init(void)
{
  /*
   * SDRAM Refresh Rate Setting
   */
  *pEBIU_SDRRC = 0x307;

  /*
   * SDRAM Memory Bank Control Register
   */
  *pEBIU_SDBCTL =
    EBCAW_9     | // SDRAM External Bank Column Address Width   =  9 Bits
    EBSZ_32     | // SDRAM External Bank Size                   = 32 MB
    EBE         ; // Enable SDRAM External Bank

  /*
   * SDRAM Memory Global Control Register
   */
  *pEBIU_SDGCTL =
    ~CDDBG      & // Disabled : Tristate SDRAM Controls During Bus Grant
    ~TCSR       & // Disabled : Temp-Compensated Self-Refresh Value (85 Deg C)
    ~EMREN      & // Disabled : Extended Mode Register
    ~FBBRW      & // Disabled : Fast Back-To-Back Read To Write
    ~EBUFE      & // Disabled : External Buffering Timing
    ~SRFS       & // Disabled : SDRAM Self-Refresh Mode
    ~PSM        & // Disabled : Power-Up Sequence (Mode Register Before Refresh)
    ~PUPSD      & // Disabled : Power-Up Start Delay (15 SCLK Cycles Delay)
    PSS         | // Enabled  : Power-Up Sequence on Next SDRAM Access
    TWR_2       | // Selected : SDRAM tWR = 2 cycles
    TRCD_3      | // Selected : SDRAM tRCD = 3 cycles
    TRP_2       | // Selected : SDRAM tRP = 2 cycles
    TRAS_6      | // Selected : SDRAM tRAS = 6 cycles
    PASR_ALL    | // Selected : All 4 SDRAM Banks Refreshed In Self-Refresh
    CL_2        | // Selected : SDRAM CAS Latency = 2 cycles
    SCTLE       ; // Enabled  : SDRAM signals
}

static void mruby_test(void)
{
#if 0
  mrb_state *mrb = mrb_open();
  mrb_close(mrb);
#endif
}

static void uart_test(void)
{
  int i;
  for (i = 0; i < 10; i++) {
    cpu_uart_putc('a');
    cpu_uart_putc('b');
    cpu_uart_putc('c');
  }
}

static void sdram_junk_test(void)
{
  unsigned int volatile *reg = (unsigned int *)0x00000000;
  int i;
  for (i = 0; i < 32; i++) {
    *(reg + i) = i;
  }
  for (i = 0; i < 32; i++) {
    if (*(reg + i) != i) {
      cpu_uart_putc('N');
      cpu_uart_putc('G');
      cpu_uart_putc('!');
      return;
    }
  }
  cpu_uart_putc('O');
  cpu_uart_putc('K');
  cpu_uart_putc('!');
}

int main(void)
{
  cpu_init();

#if 0
  sdram_init();
  sdram_junk_test();
#endif

  mruby_test();
  uart_test();

  while (1) {
  }

  return 0;
}


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
#include "cpu.h"
//#include "mruby.h"

static void init_sdram(void)
{
  //SDRAM Refresh Rate Setting
  *pEBIU_SDRRC = 0x307;

  //SDRAM Memory Bank Control Register
  *pEBIU_SDBCTL =
    EBCAW_10  | //Page size 1024
    EBSZ_32   | //32 MB of SDRAM
    EBE;   //SDRAM enable

  //SDRAM Memory Global Control Register
  *pEBIU_SDGCTL =
    ~CDDBG  & // Control disable during bus grant off
    ~TCSR & // Temperature compensated self-refresh off
    ~EMREN  & // Extended mode register enabled off
    ~FBBRW & // Fast back to back read to write off
    ~EBUFE & // External buffering enabled off
    ~SRFS & // Self-refresh setting off
    ~PSM & // Powerup sequence mode (PSM) first
    ~PUPSD & // Powerup start delay (PUPSD) off
    PSS  | // Powerup sequence start enable (PSSE) on
    TWR_2 | // Write to precharge delay TWR = 2 (14-15 ns)
    TRCD_2 | // RAS to CAS delay TRCD =2 (15-20ns)
    TRP_2 | // Bank precharge delay TRP = 2 (15-20ns)
    TRAS_4 | // Bank activate command delay TRAS = 4
    PASR_ALL| // Partial array self refresh
    CL_3 | // CAS latency
    SCTLE ; // SDRAM clock enable
}

static void sdram_test(void)
{
  unsigned int volatile *memptr = (unsigned int *)0x00000000;
  int i;
  for (i = 0; i < 16; i++) {
    *(memptr + i) = i;
  }
  for (i = 0; i < 16; i++) {
    if (*(memptr + i) != i) {
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

static void mruby_test(void)
{
//  mrb_state *mrb = mrb_open();
//  mrb_close(mrb);
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

int main(void)
{
  cpu_init();
  init_sdram();

  sdram_test();
  mruby_test();
  uart_test();

  while (1) {
  }

  return 0;
}


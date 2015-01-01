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

#include "cpu.h"
//#include "mruby.h"

static void sdram_test(void)
{
  unsigned int volatile *memptr = (unsigned int *)0x00000000;
  int i;
  for (i = 0; i < 16; i++) {
    *memptr = i;
  }
  for (i = 0; i < 16; i++) {
    if (*memptr != i) {
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

  sdram_test();
  mruby_test();
  uart_test();

  while (1) {
  }

  return 0;
}


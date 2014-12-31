/**
 * @file cpu.c
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

#include <stdint.h>
#include <cdefBF512.h>
#include <builtins.h>
#include "cpu.h"

#define SCLOCK_HZ       (100000000)
#define UART0_BAUDRATE  (57600)
#define UART0_DIVISOR   (SCLOCK_HZ / 16 / UART0_BAUDRATE)

static void setup_pll(uint8_t mul_val, uint8_t div_val)
{
  *pSIC_IWR = IWR_ENABLE(0);
  *pPLL_DIV = div_val;
  *pPLL_CTL = (mul_val) << 9;
  asm("cli r0; csync; idle; sti r0;": : :"R0");
  *pSIC_IWR = IWR_ENABLE_ALL;
}

static void init_uart(void)
{
  *pUART0_GCTL = 1;

  *pUART0_LCR |= DLAB;
  *pUART0_DLL = UART0_DIVISOR & 0xFF;
  *pUART0_DLH = UART0_DIVISOR >> 8;
  *pUART0_LCR &= ~DLAB;

  *pUART0_LCR = WLS(8);

  *pUART0_IER = 0;

  *pPORTF_MUX &= ~(PG9 | PG10);
  *pPORTF_FER |=  (PG9 | PG10);
}

/**
 * @brief Initialize the target board.
 */
void cpu_init(void)
{
  setup_pll(16, 4);
  init_uart();
}

/**
 * @brief get the character from the UART port.
 */
char cpu_uart_getc(void)
{
  while ((*pUART0_LSR & DR) == 0) {
    ssync();
  }
  return *pUART0_RBR;
}

/**
 * @brief put the character to the UART port.
 *
 * @param c Character.
 */
void cpu_uart_putc(char c)
{
  while ((*pUART0_LSR & THRE) == 0) {
    ssync();
  }
  *pUART0_THR = c;
  ssync();
}


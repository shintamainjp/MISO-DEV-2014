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

#include <stdint.h>
#include <cdefBF512.h>
#include <builtins.h>

static void SetSDRAM(void)
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

#if 0
static void StatusSDRAM(void)
{
  /* Bus Grant Status      */
  if ((*pEBIU_SDSTAT & BGSTAT)==BGSTAT) {
    printf("\n Bus granted \n");
  } else {
    printf("\n Bus not granted \n");
  }


  /* SDRAM EAB Sticky Error Status  */
  if ((*pEBIU_SDSTAT & SDEASE)==SDEASE) {
    printf(" EAB access generated an error \n");
  } else {
    printf(" No error detected \n");
  }

  /* SDRAM Will Power-Up On Next Access */
  if ((*pEBIU_SDSTAT & SDRS)==SDRS) {
    printf(" Will power up on next SDRAM access  \n");
  } else {
    printf(" Will not power up on next SDRAM access / SDRAM already powered up\n");
  }

  /* SDRAM Controller Idle     */
  if ((*pEBIU_SDSTAT & SDCI)==SDCI) {
    printf(" SDC is idle \n");
  } else {
    printf(" SDC is busy \n");
  }

  /* SDRAM Self-Refresh Active   */
  if ((*pEBIU_SDSTAT & SDSRA)==SDSRA) {
    printf(" SDRAM in self refresh mode\n");
  } else {
    printf(" SDRAM not in self refresh mode \n");
  }

  /* SDRAM Power-Up Active     */
  if ((*pEBIU_SDSTAT & SDPUA)==SDPUA) {
    printf(" SDC in powerup sequence \n");
  } else {
    printf(" SDC not in powerup sequence \n");
  }
}
#endif

static void setup_pll(uint8_t mul_val, uint8_t div_val)
{
  *pSIC_IWR = IWR_ENABLE(0);
  *pPLL_DIV = div_val;
  *pPLL_CTL = (mul_val) << 9;
  asm("cli r0; csync; idle; sti r0;": : :"R0");
  *pSIC_IWR = IWR_ENABLE_ALL;
}

int main(void)
{
  setup_pll(16, 4);
  SetSDRAM();
#if 0
  StatusSDRAM();
#endif
  return 0;
}


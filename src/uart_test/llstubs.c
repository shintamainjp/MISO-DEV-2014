/**
 * @file llstubs.c
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

/**
 * @note Low Level Stubs for Newlib.
 *
 * @see http://blackfin.uclinux.org/doku.php?id=toolchain:bare_metal:newlib
 * @see https://docs.blackfin.uclinux.org/doku.php?id=toolchain:bare_metal:compile
 * @see https://docs.blackfin.uclinux.org/doku.php?id=toolchain:bare_metal:link
 */

#include <stdio.h>
#include <errno.h>
#include <cdefBF512.h>
#include <sys/stat.h>
#include <gcc.h>

int _open(const char *name, int flags, int mode)
{
  errno = ENOSYS;
  return -1;
}

int _close(int file)
{
  errno = EBADF;
  return -1;
}

int _write(int file, char *buf, int nbytes)
{
  int i;

  for (i = 0; i < nbytes; i++) {
    cpu_uart_putc(*buf++);
  }

  return nbytes;
}

int _read(int file, char *buf, int nbytes)
{
  int i;

  for (i = 0; i < nbytes; i++) {
    buf[i] = cpu_uart_getc();
  }

  return nbytes;
}

int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return  0;
}

int _lseek(int file, int offset, int whence)
{
  return  0;
}

int _isatty(int file)
{
  return  1;
}


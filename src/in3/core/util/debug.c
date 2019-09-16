#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef __ZEPHYR__
  #include <zephyr.h>
  #define printX printk
  #define vprintX vprintk
#else
  #ifdef __NRF_FREERTOS__
    #include "SEGGER_RTT.h"
    #define __segger_printf(...) SEGGER_RTT_printf(0, __VA_ARGS__)
    #define printX __segger_printf
    #define vprintX __segger_printf
  #else
    #define printX printf
    #define vprintX vprintf
    #endif
#endif

void __dbg_log(int raw, char* file, const char* func, int line, char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char* d = strrchr(file, '/');
  d       = d ? d + 1 : file;

  if (!raw)
    printX("(%s) %s():%d - ", d, func, line);
  vprintX(fmt, args);

  va_end(args);
}

void msg_dump(const char* s, unsigned char* data, unsigned len) {
  unsigned i;

  printX("%s: ", s);
  for (i = 0U; i < len; i++) printX("%02x ", data[i]);
  printX("(%u bytes)\n", len);
}

#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct Format *supercop_formats;

void __attribute__ ((constructor)) fmt_0x00_register() {
  struct Format *fmt = calloc(sizeof(struct Format), 1);
  fmt->next   = supercop_formats;
  fmt->name   = calloc(1,5);
  fmt->detect = fmt_0x00_detect;
  fmt->encode = fmt_0x00_encode;
  fmt->decode = fmt_0x00_decode;
  supercop_formats = fmt;
  strncpy(fmt->name, "0x00", 4);
}

#ifdef __cplusplus
} // extern "C"
#endif

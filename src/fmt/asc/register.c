#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct Format *supercop_formats;

void __attribute__ ((constructor)) fmt_asc_register() {
  struct Format *fmt = calloc(1, sizeof(struct Format));
  if (!fmt) return;
  fmt->next   = supercop_formats;
  fmt->name   = calloc(1, 4);
  if (!fmt->name) {
    free(fmt);
    return;
  }
  fmt->detect = fmt_asc_detect;
  fmt->encode = fmt_asc_encode;
  fmt->decode = fmt_asc_decode;
  supercop_formats = fmt;
  strcpy(fmt->name, "asc");
}

#ifdef __cplusplus
} // extern "C"
#endif

#include <stddef.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

char fmt_0x00_detect(unsigned char *cipherdata, size_t len) {
  // Only claim 0x00-prefixed payloads of a known size, so a future format
  // sharing the leading null-byte but with a different length won't collide
  if (len != 33 && len != 97) return 0;
  if ((*cipherdata) == '\0') return 1;
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

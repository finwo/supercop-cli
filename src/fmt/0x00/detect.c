#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

char fmt_0x00_detect(unsigned char *cipherdata) {
  if ((*cipherdata) == '\0') return 1;
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

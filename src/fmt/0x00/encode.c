#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

char * fmt_0x00_encode(struct KeyPair *kp, int *len) {
  *len = 1 + 32 + 64;
  char *result = malloc(*len);
  result[0]    = 0;
  memcpy(result +  1, kp->public_key , 32);
  memcpy(result + 33, kp->private_key, 64);
  return result;
}

#ifdef __cplusplus
} // extern "C"
#endif

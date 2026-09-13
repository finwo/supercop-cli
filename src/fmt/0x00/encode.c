#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

char * fmt_0x00_encode(struct KeyPair *kp, size_t *len) {
  char *result;
  if (!kp || !kp->public_key || !len) return NULL;
  result = malloc(1 + 32 + 64);
  if (!result) return NULL;
  result[0] = 0;
  memcpy(result + 1, kp->public_key, 32);
  *len = 1 + 32;
  if (kp->private_key) {
    memcpy(result + 33, kp->private_key, 64);
    *len = 1 + 32 + 64;
  }
  return result;
}

#ifdef __cplusplus
} // extern "C"
#endif

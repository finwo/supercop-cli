#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct KeyPair * fmt_0x00_decode(unsigned char *cipherdata) {
  struct KeyPair *kp = calloc(1, sizeof(struct KeyPair));
  kp->public_key  = calloc(1, 32);
  kp->private_key = calloc(1, 64);
  memcpy(kp->public_key , cipherdata +  1, 32);
  memcpy(kp->private_key, cipherdata + 33, 64);
  return kp;
}

#ifdef __cplusplus
} // extern "C"
#endif

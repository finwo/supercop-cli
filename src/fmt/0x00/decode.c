#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct KeyPair * fmt_0x00_decode(unsigned char *cipherdata, size_t len) {
  struct KeyPair *kp;
  // Pub-only (33 bytes) or full (97 bytes), anything else is invalid
  if (len != 33 && len != 97) return NULL;
  kp = calloc(1, sizeof(struct KeyPair));
  if (!kp) return NULL;
  kp->public_key  = calloc(1, 32);
  if (!kp->public_key) {
    free(kp);
    return NULL;
  }
  memcpy(kp->public_key , cipherdata +  1, 32);
  if (len == 97) {
    kp->private_key = calloc(1, 64);
    if (!kp->private_key) {
      free(kp->public_key);
      free(kp);
      return NULL;
    }
    memcpy(kp->private_key, cipherdata + 33, 64);
  }
  return kp;
}

#ifdef __cplusplus
} // extern "C"
#endif

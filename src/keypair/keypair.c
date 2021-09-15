#include <stdlib.h>
/* #include <string.h> */

#include "keypair.h"

#ifdef __cplusplus
extern "C" {
#endif

void keypair_free(struct KeyPair *kp) {
  free(kp->public_key);
  free(kp->private_key);
  free(kp);
}

#ifdef __cplusplus
} // extern "C"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

static void hdr_hex_encode(char *dst, const unsigned char *src, size_t n) {
  static const char digits[] = "0123456789abcdef";
  size_t i;
  for (i = 0; i < n; i++) {
    dst[i * 2]     = digits[(src[i] >> 4) & 0xF];
    dst[i * 2 + 1] = digits[src[i] & 0xF];
  }
  dst[n * 2] = '\0';
}

char * fmt_hdr_encode(struct KeyPair *kp, size_t *len) {
  char pubhex[65];
  char privhex[129];
  const char *privstr;
  char *out;
  size_t need;
  int n;

  if (!kp || !kp->public_key || !len) return NULL;
  hdr_hex_encode(pubhex, kp->public_key, 32);
  if (kp->private_key) {
    hdr_hex_encode(privhex, kp->private_key, 64);
    privstr = privhex;
  } else {
    privstr = "(no private key)";
  }

  need = strlen("public-key: \nprivate-key: \n") + 64 + strlen(privstr) + 1;
  out = malloc(need);
  if (!out) return NULL;
  n = snprintf(out, need, "public-key: %s\nprivate-key: %s\n", pubhex, privstr);
  if (n < 0 || (size_t)n >= need) {
    free(out);
    return NULL;
  }
  *len = (size_t)n;
  return out;
}

#ifdef __cplusplus
} // extern "C"
#endif

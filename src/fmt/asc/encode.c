#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "armor.h"
#include "b64.h"

#ifdef __cplusplus
extern "C" {
#endif

// Wrap raw bytes in an ascii-armor block; returns malloc'd NUL-terminated text.
static char *asc_wrap(const char *label, const unsigned char *raw, size_t rawlen,
                      size_t *outlen) {
  char *b64, *out;
  size_t b64len, need;
  int n;

  b64 = asc_b64_encode(raw, rawlen, &b64len);
  if (!b64) return NULL;
  // "-----BEGIN %s-----\n" + b64 + "-----END %s-----\n" + NUL
  need = strlen("-----BEGIN -----\n-----END -----\n") + 2 * strlen(label) + b64len + 1;
  out = malloc(need);
  if (!out) {
    free(b64);
    return NULL;
  }
  n = snprintf(out, need, "-----BEGIN %s-----\n%s-----END %s-----\n",
               label, b64, label);
  free(b64);
  if (n < 0 || (size_t)n >= need) {
    free(out);
    return NULL;
  }
  *outlen = (size_t)n;
  return out;
}

char * fmt_asc_encode(struct KeyPair *kp, size_t *len) {
  char *priv = NULL, *pub = NULL, *out;
  size_t privlen = 0, publen = 0;

  if (!kp || !kp->public_key || !len) return NULL;
  // Armor carries just the key bytes, no prefix. A full pair emits both a
  // PRIVATE block (64 private bytes) and a PUBLIC block (32 public bytes);
  // a pub-only pair emits just the PUBLIC block.
  if (kp->private_key) {
    priv = asc_wrap(ASC_LABEL_PRIVATE, kp->private_key, 64, &privlen);
    if (!priv) return NULL;
  }
  pub = asc_wrap(ASC_LABEL_PUBLIC, kp->public_key, 32, &publen);
  if (!pub) {
    if (priv) free(priv);
    return NULL;
  }
  out = malloc(privlen + publen + 1);
  if (!out) {
    if (priv) free(priv);
    if (pub) free(pub);
    return NULL;
  }
  memcpy(out, priv, privlen);
  memcpy(out + privlen, pub, publen);
  out[privlen + publen] = '\0';
  free(priv);
  free(pub);
  *len = privlen + publen;
  return out;
}

#ifdef __cplusplus
} // extern "C"
#endif

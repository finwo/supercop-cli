#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "armor.h"
#include "b64.h"
#include "orlp/ed25519.h"

#ifdef __cplusplus
extern "C" {
#endif

// Decode one armor block into malloc'd raw bytes; NULL unless the block
// exists, is valid base64, and decodes to exactly wantlen bytes.
static unsigned char *asc_block_bytes(unsigned char *cipherdata, size_t len,
                                      const char *label, size_t wantlen) {
  const unsigned char *payload;
  size_t payloadlen;
  unsigned char *raw;
  size_t rawlen;
  if (!asc_find_block(cipherdata, len, label, &payload, &payloadlen)) return NULL;
  raw = asc_b64_decode(payload, payloadlen, &rawlen);
  if (!raw || rawlen != wantlen) {
    if (raw) free(raw);
    return NULL;
  }
  return raw;
}

static struct KeyPair *asc_pair_alloc(void) {
  struct KeyPair *kp = calloc(1, sizeof(struct KeyPair));
  if (!kp) return NULL;
  kp->public_key = calloc(1, 32);
  if (!kp->public_key) {
    free(kp);
    return NULL;
  }
  return kp;
}

struct KeyPair * fmt_asc_decode(unsigned char *cipherdata, size_t len) {
  unsigned char *privraw;
  unsigned char *pubraw;
  struct KeyPair *kp;

  // A PRIVATE block is authoritative for full keys: present-but-corrupt
  // fails closed instead of silently downgrading to pub-only.
  if (asc_find_block(cipherdata, len, ASC_LABEL_PRIVATE, NULL, NULL)) {
    privraw = asc_block_bytes(cipherdata, len, ASC_LABEL_PRIVATE, 64);
    if (!privraw) return NULL;
    kp = asc_pair_alloc();
    if (!kp) {
      free(privraw);
      return NULL;
    }
    kp->private_key = calloc(1, 64);
    if (!kp->private_key) {
      free(privraw);
      free(kp->public_key);
      free(kp);
      return NULL;
    }
    memcpy(kp->private_key, privraw, 64);
    if (privraw) free(privraw);
    ed25519_derive_pubkey(kp->public_key, kp->private_key);
    // An accompanying PUBLIC block must agree when present
    pubraw = asc_block_bytes(cipherdata, len, ASC_LABEL_PUBLIC, 32);
    if (pubraw) {
      int ok = memcmp(pubraw, kp->public_key, 32) == 0;
      free(pubraw);
      if (!ok) {
        keypair_free(kp);
        return NULL;
      }
    }
    return kp;
  }

  // Lone PUBLIC block (32 public bytes) decodes as a pub-only pair
  pubraw = asc_block_bytes(cipherdata, len, ASC_LABEL_PUBLIC, 32);
  if (!pubraw) return NULL;
  kp = asc_pair_alloc();
  if (!kp) {
    free(pubraw);
    return NULL;
  }
  memcpy(kp->public_key, pubraw, 32);
  if (pubraw) free(pubraw);
  return kp;
}

#ifdef __cplusplus
} // extern "C"
#endif

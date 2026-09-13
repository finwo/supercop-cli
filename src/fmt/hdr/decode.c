#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "scan.h"
#include "orlp/ed25519.h"

#ifdef __cplusplus
extern "C" {
#endif

static int hdr_hexval(unsigned char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

// Parse exactly hexlen hex chars into out; returns 1 on success.
static int hdr_parse_hex(const unsigned char *val, size_t vallen,
                         unsigned char *out, size_t hexlen) {
  size_t i;
  int hi, lo;
  if (vallen != hexlen * 2) return 0;
  for (i = 0; i < hexlen; i++) {
    hi = hdr_hexval(val[i * 2]);
    lo = hdr_hexval(val[i * 2 + 1]);
    if (hi < 0 || lo < 0) return 0;
    out[i] = (unsigned char)((hi << 4) | lo);
  }
  return 1;
}

struct KeyPair * fmt_hdr_decode(unsigned char *cipherdata, size_t len) {
  const unsigned char *pos = cipherdata;
  const unsigned char *end = cipherdata + len;
  const unsigned char *line;
  size_t linelen;
  int which;
  const unsigned char *val;
  size_t vallen;
  unsigned char pub[32];
  unsigned char priv[64];
  int seen_pub = 0, have_pub = 0;
  int seen_priv = 0, have_priv = 0;
  struct KeyPair *kp;

  // First valid occurrence of each label wins; malformed hex runs are
  // ignored so a later valid block in the stream can still win.
  while (hdr_next_line(&pos, end, &line, &linelen)) {
    if (!hdr_match_label(line, linelen, &which, &val, &vallen)) continue;
    if (which == 1 && seen_pub) continue;
    if (which == 2 && seen_priv) continue;
    if (vallen == 0) continue;
    // Parenthesized value marks the field absent (e.g. "(no private key)")
    if (val[0] == '(') {
      if (which == 1) seen_pub = 1;
      else seen_priv = 1;
      continue;
    }
    if (which == 1) {
      if (!hdr_parse_hex(val, vallen, pub, 32)) continue;
      seen_pub = 1;
      have_pub = 1;
    } else {
      if (!hdr_parse_hex(val, vallen, priv, 64)) continue;
      seen_priv = 1;
      have_priv = 1;
    }
  }

  if (!have_pub && !have_priv) return NULL;

  // A lone private key still yields its public half via scalar mult
  if (!have_pub) {
    ed25519_derive_pubkey(pub, priv);
    have_pub = 1;
  }

  kp = calloc(1, sizeof(struct KeyPair));
  if (!kp) return NULL;
  kp->public_key = calloc(1, 32);
  if (!kp->public_key) {
    free(kp);
    return NULL;
  }
  memcpy(kp->public_key, pub, 32);
  if (have_priv) {
    kp->private_key = calloc(1, 64);
    if (!kp->private_key) {
      free(kp->public_key);
      free(kp);
      return NULL;
    }
    memcpy(kp->private_key, priv, 64);
  }
  return kp;
}

#ifdef __cplusplus
} // extern "C"
#endif

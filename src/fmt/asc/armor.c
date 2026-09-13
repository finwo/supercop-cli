#include <stdio.h>
#include <string.h>

#include "armor.h"

#ifdef __cplusplus
extern "C" {
#endif

int asc_find_block(const unsigned char *buf, size_t len, const char *label,
                   const unsigned char **payload, size_t *payloadlen) {
  char begin[64], end[64];
  size_t blen, elen, i, pend;
  int n;

  if (!buf || !label) return 0;
  n = snprintf(begin, sizeof(begin), "-----BEGIN %s-----", label);
  if (n < 0 || (size_t)n >= sizeof(begin)) return 0;
  n = snprintf(end, sizeof(end), "-----END %s-----", label);
  if (n < 0 || (size_t)n >= sizeof(end)) return 0;
  blen = strlen(begin);
  elen = strlen(end);
  if (len < blen) return 0;

  for (i = 0; i + blen <= len; i++) {
    size_t pstart;
    if (memcmp(buf + i, begin, blen) != 0) continue;
    pstart = i + blen;
    for (pend = pstart; pend + elen <= len; pend++) {
      if (memcmp(buf + pend, end, elen) == 0) {
        if (payload) *payload = buf + pstart;
        if (payloadlen) *payloadlen = pend - pstart;
        return 1;
      }
    }
    // Begin marker without an end: no valid block
    return 0;
  }
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

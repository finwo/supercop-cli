#include <stddef.h>

#include "common.h"
#include "scan.h"

#ifdef __cplusplus
extern "C" {
#endif

static int hdr_is_hex(unsigned char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

char fmt_hdr_detect(unsigned char *cipherdata, size_t len) {
  const unsigned char *pos = cipherdata;
  const unsigned char *end = cipherdata + len;
  const unsigned char *line;
  size_t linelen;
  int which;
  const unsigned char *val;
  size_t vallen;

  // Claim the buffer when any key-labeled line with a non-empty value
  // appears anywhere in it; surrounding stream content is irrelevant.
  while (hdr_next_line(&pos, end, &line, &linelen)) {
    if (!hdr_match_label(line, linelen, &which, &val, &vallen)) continue;
    if (vallen == 0) continue;
    // A hex run or a parenthesized absent-marker both count as values
    if (val[0] == '(' || hdr_is_hex(val[0])) return 1;
  }
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

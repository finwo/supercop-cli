#include <stddef.h>

#include "scan.h"

#ifdef __cplusplus
extern "C" {
#endif

static int hdr_is_ws(unsigned char c) {
  return c == ' ' || c == '\t' || c == '\r';
}

static int hdr_lower(unsigned char c) {
  if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
  return c;
}

// Case-insensitive literal match, bounded by remaining length.
static int hdr_match_word(const unsigned char *p, size_t left, const char *word) {
  size_t i = 0;
  while (word[i]) {
    if (i >= left) return 0;
    if (hdr_lower(p[i]) != (unsigned char)word[i]) return 0;
    i++;
  }
  return 1;
}

int hdr_match_label(const unsigned char *line, size_t linelen, int *which,
                    const unsigned char **val, size_t *vallen) {
  size_t i = 0;
  int w = 0;
  size_t wlen = 0;

  while (i < linelen && hdr_is_ws(line[i])) i++;

  if (hdr_match_word(line + i, linelen - i, "public-key")) {
    w = 1;
    wlen = 10;
  } else if (hdr_match_word(line + i, linelen - i, "private-key")) {
    w = 2;
    wlen = 11;
  } else {
    return 0;
  }
  i += wlen;

  while (i < linelen && hdr_is_ws(line[i])) i++;
  if (i >= linelen || line[i] != ':') return 0;
  i++;

  while (i < linelen && hdr_is_ws(line[i])) i++;

  *which = w;
  *val = line + i;
  *vallen = linelen - i;
  while (*vallen > 0 && hdr_is_ws((*val)[*vallen - 1])) (*vallen)--;
  return 1;
}

int hdr_next_line(const unsigned char **pos, const unsigned char *end,
                  const unsigned char **line, size_t *linelen) {
  const unsigned char *nl;
  if (*pos >= end) return 0;
  *line = *pos;
  nl = *pos;
  while (nl < end && *nl != '\n') nl++;
  *linelen = (size_t)(nl - *pos);
  // Strip a trailing carriage return so CRLF streams parse cleanly
  if (*linelen > 0 && (*pos)[*linelen - 1] == '\r') (*linelen)--;
  *pos = (nl < end) ? nl + 1 : end;
  return 1;
}

#ifdef __cplusplus
} // extern "C"
#endif

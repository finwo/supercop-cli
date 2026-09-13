#include <stdlib.h>

#include "b64.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char asc_b64_alphabet[] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *asc_b64_encode(const unsigned char *in, size_t len, size_t *outlen) {
  size_t rawlen, full_lines, i, o, raw;
  char *out;
  unsigned int n;

  if (!in || !outlen || len == 0) return NULL;
  // 4 chars per 3 bytes, plus '\n' per 64-char line, plus trailing '\n'
  rawlen = ((len + 2) / 3) * 4;
  full_lines = (rawlen + 63) / 64;
  out = malloc(rawlen + full_lines + 1);
  if (!out) return NULL;

  o = 0;
  raw = 0;
  for (i = 0; i < len; i += 3) {
    size_t rem = len - i;
    n = (unsigned int)in[i] << 16;
    if (rem > 1) n |= (unsigned int)in[i + 1] << 8;
    if (rem > 2) n |= in[i + 2];
    out[o++] = asc_b64_alphabet[(n >> 18) & 0x3F];
    out[o++] = asc_b64_alphabet[(n >> 12) & 0x3F];
    out[o++] = (rem > 1) ? asc_b64_alphabet[(n >> 6) & 0x3F] : '=';
    out[o++] = (rem > 2) ? asc_b64_alphabet[n & 0x3F] : '=';
    raw += 4;
    if (raw % 64 == 0) out[o++] = '\n';
  }
  if (out[o - 1] != '\n') out[o++] = '\n';
  out[o] = '\0';
  *outlen = o;
  return out;
}

static int asc_b64_value(unsigned char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a' + 26;
  if (c >= '0' && c <= '9') return c - '0' + 52;
  if (c == '+') return 62;
  if (c == '/') return 63;
  return -1;
}

static int asc_b64_is_ws(unsigned char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

unsigned char *asc_b64_decode(const unsigned char *in, size_t len, size_t *outlen) {
  size_t cleanlen = 0, i, o;
  unsigned char *clean, *out;
  unsigned int n;
  int a, b, c, d;

  if (!in || !outlen || len == 0) return NULL;
  clean = malloc(len);
  if (!clean) return NULL;
  for (i = 0; i < len; i++) {
    if (asc_b64_is_ws(in[i])) continue;
    if (in[i] == '=' || asc_b64_value(in[i]) >= 0) {
      clean[cleanlen++] = in[i];
    } else {
      free(clean);
      return NULL;
    }
  }
  // Input must arrive in complete 4-char quanta
  if (cleanlen == 0 || cleanlen % 4 != 0) {
    free(clean);
    return NULL;
  }
  out = malloc((cleanlen / 4) * 3);
  if (!out) {
    free(clean);
    return NULL;
  }
  o = 0;
  for (i = 0; i < cleanlen; i += 4) {
    a = asc_b64_value(clean[i]);
    b = asc_b64_value(clean[i + 1]);
    c = (clean[i + 2] == '=') ? 0 : asc_b64_value(clean[i + 2]);
    d = (clean[i + 3] == '=') ? 0 : asc_b64_value(clean[i + 3]);
    // Padding only valid as the last quantum's tail
    if (a < 0 || b < 0 || c < 0 || d < 0 ||
        clean[i] == '=' || clean[i + 1] == '=' ||
        (clean[i + 2] == '=' && clean[i + 3] != '=') ||
        ((clean[i + 2] == '=' || clean[i + 3] == '=') && i + 4 != cleanlen)) {
      free(clean);
      free(out);
      return NULL;
    }
    n = ((unsigned int)a << 18) | ((unsigned int)b << 12) |
        ((unsigned int)c << 6) | (unsigned int)d;
    out[o++] = (n >> 16) & 0xFF;
    if (clean[i + 2] != '=') out[o++] = (n >> 8) & 0xFF;
    if (clean[i + 3] != '=') out[o++] = n & 0xFF;
  }
  free(clean);
  *outlen = o;
  return out;
}

#ifdef __cplusplus
} // extern "C"
#endif

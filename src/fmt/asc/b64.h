// Minimal internal base64 (RFC 4648 alphabet), just enough for the asc armor.
// No external dependency; operates on length-bounded buffers.

#ifndef __SUPERCOP_FMT_ASC_B64_H__
#define __SUPERCOP_FMT_ASC_B64_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Encodes in, wrapping lines at 64 columns with '\n', trailing '\n' included.
// Returns malloc'd buffer with *outlen set, or NULL on failure/empty input.
char *asc_b64_encode(const unsigned char *in, size_t len, size_t *outlen);

// Decodes in, skipping whitespace (space/tab/CR/LF). Expects correct '='
// padding. Returns malloc'd buffer with *outlen set, or NULL on failure.
unsigned char *asc_b64_decode(const unsigned char *in, size_t len, size_t *outlen);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_FMT_ASC_B64_H__

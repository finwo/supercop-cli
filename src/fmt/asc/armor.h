// Shared ascii-armor block locating for the asc format.
//
// A block looks like:
//
//   -----BEGIN <label>-----
//   <base64 payload, 64-col wrapped>
//   -----END <label>-----
//
// All scanning is bounded by the buffer length, never assuming
// NUL-termination.

#ifndef __SUPERCOP_FMT_ASC_ARMOR_H__
#define __SUPERCOP_FMT_ASC_ARMOR_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Namespaced labels: bare "PRIVATE KEY"/"PUBLIC KEY" would invite openssl
// to misparse our raw payload as DER/PKCS#8.
#define ASC_LABEL_PRIVATE "SUPERCOP PRIVATE KEY"
#define ASC_LABEL_PUBLIC  "SUPERCOP PUBLIC KEY"

// Locate the first block with the given label. On success sets
// *payload/*payloadlen to the bytes between the markers and returns 1,
// otherwise returns 0. payload/payloadlen may be NULL to just test presence.
int asc_find_block(const unsigned char *buf, size_t len, const char *label,
                   const unsigned char **payload, size_t *payloadlen);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_FMT_ASC_ARMOR_H__

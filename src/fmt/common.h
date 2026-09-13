#ifndef __SUPERCOP_FMT_COMMON_H__
#define __SUPERCOP_FMT_COMMON_H__

#include <stddef.h>

#include "../keypair/keypair.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Format {
  void *next;
  char *name;
  char            (*detect)(unsigned char *, size_t);
  char           *(*encode)(struct KeyPair *, size_t*);
  struct KeyPair *(*decode)(unsigned char *, size_t);
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_FMT_COMMON_H__

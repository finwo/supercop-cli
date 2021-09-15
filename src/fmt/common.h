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
  char            (*detect)(unsigned char *);
  char           *(*encode)(struct KeyPair *, int*);
  struct KeyPair *(*decode)(unsigned char *);
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_FMT_COMMON_H__

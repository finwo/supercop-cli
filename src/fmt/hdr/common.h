#include "../common.h"
#include "../../keypair/keypair.h"

#ifndef __SUPERCOP_FMT_HDR_COMMON_H__
#define __SUPERCOP_FMT_HDR_COMMON_H__

char fmt_hdr_detect(unsigned char *cipherdata, size_t len);
char * fmt_hdr_encode(struct KeyPair *keypair, size_t *len);
struct KeyPair * fmt_hdr_decode(unsigned char *cipherdata, size_t len);

#endif // __SUPERCOP_FMT_HDR_COMMON_H__

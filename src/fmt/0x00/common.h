#include "../common.h"
#include "../../keypair/keypair.h"

#ifndef __SUPERCOP_FMT_0X00_COMMON_H__
#define __SUPERCOP_FMT_0X00_COMMON_H__

char fmt_0x00_detect(unsigned char *cipherdata);
char * fmt_0x00_encode(struct KeyPair *keypair, int *len);
struct KeyPair * fmt_0x00_decode(unsigned char *cipherdata);

#endif // __SUPERCOP_FMT_0X00_COMMON_H__

#ifndef __SUPERCOP_KEYPAIR_KEYPAIR_H__
#define __SUPERCOP_KEYPAIR_KEYPAIR_H__

#ifdef __cplusplus
extern "C" {
#endif

struct KeyPair {
  unsigned char *public_key;
  unsigned char *private_key;
};

void keypair_free(struct KeyPair *kp);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_KEYPAIR_KEYPAIR_H__

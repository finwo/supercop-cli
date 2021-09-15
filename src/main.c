#ifdef __cplusplus
extern "C" {
#endif

#define COMMAND_GENERATE 1
#define COMMAND_PUBKEY   2
#define COMMAND_SIGN     4
#define COMMAND_VERIFY   8

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "argparse.h"
#include "ed25519.h"

static const char *const usage[] = {
  "supercop <command> [options]",
  NULL,
};

int cmd_generate(FILE *fd) {
  unsigned char version = 0;
  unsigned char seed[32];
  unsigned char public_key[32];
  unsigned char private_key[64];

  if (ed25519_create_seed(seed)) {
    fprintf(stderr, "Error while generating seed\n");
    return 1;
  }

  ed25519_create_keypair(public_key, private_key, seed);

  // Hardcoded to format 0x00
  fwrite(&version, 1, 1, fd);
  fwrite(public_key, 1, 32, fd);
  fwrite(private_key, 1, 64, fd);

  return 0;
}

int cmd_pubkey() {
  return 0;
}

int cmd_sign() {
  return 0;
}
int cmd_verify() {
  return 0;
}

int main(int argc, const char **argv) {
  const char *identityFile    = NULL;
  const char *verifySignature = NULL;
  const char *message         = NULL;
  const char *messageFile     = NULL;
  int command = 0;
  int result  = 0;

  FILE *fidentity = NULL;
  FILE *fmessage  = NULL;
  FILE *fout      = NULL;

  // Setup help & argument parsing
  struct argparse_option options[] = {
    OPT_GROUP("Operations"),
    OPT_BIT(0, "pubkey"  , &command, "Generate new key"          , NULL, COMMAND_GENERATE),
    OPT_BIT(0, "generate", &command, "Generate new key"          , NULL, COMMAND_GENERATE),
    OPT_BIT(0, "sign"    , &command, "Sign a message"            , NULL, COMMAND_SIGN    ),
    OPT_BIT(0, "verify"  , &command, "Verify a message signature", NULL, COMMAND_VERIFY  ),
    OPT_GROUP("Basic options"),
    OPT_HELP(),
    OPT_STRING('i', "identity-file", &identityFile   , "Select identity file to use for the operation"),
    OPT_STRING('m', "message"      , &message        , "Message to sign or verify (defaults to stdin)"),
    OPT_STRING('M', "message-file" , &messageFile    , "Message file to sign or verify (defaults to stdin)"),
    OPT_STRING('s', "signature"    , &verifySignature, "Signature to verify"),
    OPT_END(),
  };

  // Setup basic information
  struct argparse argparse;
  argparse_init(&argparse, options, usage, 0);
  argparse_describe(&argparse, "\nMinimalistic program to generate ed25519 keys and verify/sign messages", "\n (c) 2021 finwo");

  // Actually parse the arguments
  argc = argparse_parse(&argparse, argc, argv);

  // Our paths separate here
  switch(command) {
    case COMMAND_GENERATE:

      // Build output fd
      fout = stdout;
      if (identityFile) {
        fout = fopen(identityFile, "w+");
        if (fout < 1) {
          fprintf(stderr, "Could not open output file");
          return 1;
        }
      }

      // Actually generate the key file
      result = cmd_generate(fout);

      // Close output fd if file
      if (identityFile) {
        fclose(fout);
      }

      // Done
      return result;

    case COMMAND_PUBKEY:
      return cmd_pubkey();
    case COMMAND_VERIFY:
      return cmd_verify();
    case COMMAND_SIGN:
      return cmd_sign();
    default:
      argparse_usage(&argparse);
      return 1;
  }

  return 42;
}

#ifdef __cplusplus
} // extern "C"
#endif

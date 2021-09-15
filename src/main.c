#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_GENERATE  1
#define COMMAND_PRINTKEY  2
#define COMMAND_SIGN      4
#define COMMAND_VERIFY    8
#define COMMAND_VERSION  16

#ifndef VERSION
#define VERSION "n/a"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "../lib/argparse/argparse.h"
#include "../lib/ed25519/src/ed25519.h"
#include "fmt/common.h"
#include "keypair/keypair.h"

struct Format *supercop_formats = NULL;

static const char *const usage[] = {
  "supercop <command> [options]",
  NULL,
};

long fremaining(FILE *fd) {
  long current = ftell(fd);
  fseek(fd, 0, SEEK_END);
  long end = ftell(fd);
  fseek(fd, current, SEEK_SET);
  return end - current;
}

struct KeyPair * readKeyFile(const char *filename) {
  struct Format *fmt = supercop_formats;
  FILE *fd = fopen(filename, "r");
  unsigned char *buf;
  struct KeyPair *kp;

  if (!fd) {
    fprintf(stderr, "Could not open key file\n");
    exit(1);
  }

  // Read whole file
  long fsize = fremaining(fd);
  buf = calloc(1, fsize + 1);
  fread(buf, 1, fsize, fd);

  // Auto-detect format
  while(fmt) {
    if (!fmt->detect(buf)) {
      fmt = fmt->next;
      continue;
    }
    kp = fmt->decode(buf);
    free(buf);
    fclose(fd);
    return kp;
  }

  return NULL;
}

int cmd_generate(FILE *fd) {
  unsigned char version = 0;
  unsigned char seed[32];
  unsigned char public_key[32];
  unsigned char private_key[64];

  // Generate random seed
  if (ed25519_create_seed(seed)) {
    fprintf(stderr, "Error while generating seed\n");
    exit(1);
  }

  // Generate the actual key
  ed25519_create_keypair(public_key, private_key, seed);
  struct KeyPair kp;
  kp.public_key  = public_key;
  kp.private_key = private_key;

  // Encode in the last-registered format
  // TODO: allow format selection
  int   encoded_length;
  char *encoded = supercop_formats->encode(&kp, &encoded_length);
  fwrite(encoded, 1, encoded_length, fd);

  // Clean up
  free(encoded);

  return 0;
}

// Prints human-readable version of the keypair
// Should be easy in high-level languages as well
int cmd_printkey(struct KeyPair *kp, FILE *fout) {
  int i;
  fprintf(fout, "public-key: ");
  for(i=0;i<32;i++) fprintf(fout, "%02x", kp->public_key[i]);
  fprintf(fout, "\nprivate-key: ");
  for(i=0;i<64;i++) fprintf(fout, "%02x", kp->private_key[i]);
  fprintf(fout, "\n");
  return 0;
}

int cmd_sign(struct KeyPair *kp, FILE *fmessage, FILE *fout) {
  int i;

  // Read message
  // TODO: make this stdin-compatible
  long message_len = fremaining(fmessage);
  const unsigned char *message = calloc(1, message_len);
  fread(message, 1, message_len, fmessage);

  // Create signature
  unsigned char *signature = calloc(1, 64);
  ed25519_sign(signature, message, message_len, kp->public_key, kp->private_key);

  // Output signature
  for(i=0;i<64;i++) fprintf(fout, "%02x", signature[i]);
  fprintf(fout, "\n");

  // Clean up
  free(signature);
  free(message);
  return 0;
}

int cmd_verify(struct KeyPair *kp, FILE *fmessage, FILE *fsignature, FILE *fout) {
  int isValid;

  // Read message
  // TODO: make this stdin-compatible
  long message_len = fremaining(fmessage);
  const unsigned char *message = calloc(1, message_len);
  fread(message, 1, message_len, fmessage);

  // Read signature
  long signature_len = fremaining(fsignature);
  const unsigned char *signature = calloc(1, signature_len);
  fread(signature, 1, signature_len, fsignature);
  if (signature_len != 64) {
    fprintf(stderr, "Invalid signature!!\n");
    exit(1);
  }

  // Verify the signature
  isValid = ed25519_verify(signature, message, message_len, kp->public_key);

  // Clean up
  free(signature);
  free(message);

  // Handle response
  if (isValid) {
    fprintf(fout, "OK\n");
    return 0;
  } else {
    fprintf(fout, "FAIL\n");
    return 1;
  }
}

int main(int argc, const char **argv) {
  struct KeyPair *kp;
  const char *keyFile         = NULL;
  const char *verifySignature = NULL;
  const char *message         = NULL;
  const char *messageFile     = NULL;
  unsigned char c;
  int command = 0;
  int result  = 0;
  int i;
  const char *pos;

  FILE *fmessage   = NULL;
  FILE *fsignature = NULL;
  FILE *fout       = NULL;

  // Setup help & argument parsing
  struct argparse_option options[] = {
    OPT_GROUP("Operations"),
    OPT_BIT(0, "printkey", &command, "Print contents of key file"  , NULL, COMMAND_PRINTKEY),
    OPT_BIT(0, "generate", &command, "Generate new key"            , NULL, COMMAND_GENERATE),
    OPT_BIT(0, "sign"    , &command, "Sign a message"              , NULL, COMMAND_SIGN    ),
    OPT_BIT(0, "verify"  , &command, "Verify a message signature"  , NULL, COMMAND_VERIFY  ),
    OPT_BIT(0, "version" , &command, "Show version number and exit", NULL, COMMAND_VERSION ),
    OPT_GROUP("Basic options"),
    OPT_HELP(),
    OPT_STRING('k', "key-file"    , &keyFile        , "Select key file to use for the operation"),
    OPT_STRING('m', "message"     , &message        , "Message to sign or verify (defaults to stdin)"),
    OPT_STRING('M', "message-file", &messageFile    , "Message file to sign or verify (defaults to stdin)"),
    OPT_STRING('s', "signature"   , &verifySignature, "Signature to verify"),
    OPT_END(),
  };

  // Setup basic information
  struct argparse argparse;
  argparse_init(&argparse, options, usage, 0);
  argparse_describe(&argparse, "\nMinimalistic program to generate ed25519 keys and verify/sign messages", NULL);

  // Actually parse the arguments
  argc = argparse_parse(&argparse, argc, argv);

  // Our paths separate here
  switch(command) {

    case COMMAND_VERSION:
      fprintf(stdout, "%s\n", VERSION);
      return 0;

    case COMMAND_GENERATE:

      // Build output fd
      fout = stdout;
      if (keyFile) {
        fout = fopen(keyFile, "w+");
        if (!fout) {
          fprintf(stderr, "Could not open output file\n");
          exit(1);
        }
      }

      // Actually generate the key file
      result = cmd_generate(fout);

      // Close output fd if file
      if (keyFile) {
        fclose(fout);
      }

      // Done
      return result;

    case COMMAND_PRINTKEY:

      // No key file = error
      if (!keyFile) {
        fprintf(stderr, "Missing required argument: key-file\n\n");
        argparse_usage(&argparse);
        exit(1);
      }

      kp = readKeyFile(keyFile);
      result = cmd_printkey(kp, stdout);
      keypair_free(kp);
      return result;

    case COMMAND_SIGN:

      // No key file = error
      if (!keyFile) {
        fprintf(stderr, "Missing required argument: key-file\n\n");
        argparse_usage(&argparse);
        exit(1);
      }

      // Fetch message as FILE*
      fmessage = stdin;
      if (message) {
        fmessage = tmpfile();
        fwrite(message, 1, strlen(message), fmessage);
        fseek(fmessage, 0, SEEK_SET);
      }
      if (messageFile) {
        if (message) fclose(fmessage);
        fmessage = fopen(messageFile, "r");
        if (!fmessage) {
          fprintf(stderr, "Could not open message file\n");
          exit(1);
        }
      }

      // Read the key file we're using
      kp = readKeyFile(keyFile);
      result = cmd_sign(kp, fmessage, stdout);
      keypair_free(kp);
      return result;

    case COMMAND_VERIFY:

      // No key file = error
      if (!keyFile) {
        fprintf(stderr, "Missing required argument: key-file\n\n");
        argparse_usage(&argparse);
        exit(1);
      }

      // Fetch message as FILE*
      fmessage = stdin;
      if (message) {
        fmessage = tmpfile();
        fwrite(message, 1, strlen(message), fmessage);
        fseek(fmessage, 0, SEEK_SET);
      }
      if (messageFile) {
        if (message) fclose(fmessage);
        fmessage = fopen(messageFile, "r");
        if (!fmessage) {
          fprintf(stderr, "Could not open message file\n");
          exit(1);
        }
      }

      // Signature = required during verify
      if (!verifySignature) {
        fprintf(stderr, "Missing required argument: signature\n\n");
        argparse_usage(&argparse);
        exit(1);
      }

      // Parse signature as hex & keep as FILE*
      fsignature = tmpfile();
      pos = verifySignature;
      while(*pos) {
        sscanf(pos, "%2hhx", &c);
        fputc(c, fsignature);
        pos += 2;
      }
      fseek(fsignature, 0, SEEK_SET);

      // Read key & verify signature
      kp = readKeyFile(keyFile);
      result = cmd_verify(kp, fmessage, fsignature, stdout);
      fclose(fsignature);
      keypair_free(kp);
      return result;
  }

  argparse_usage(&argparse);
  return 1;
}

#ifdef __cplusplus
} // extern "C"
#endif

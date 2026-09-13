#include <stdio.h>
#include <stdlib.h>

#include "cli/command.h"
#include "cli/common.h"
#include "cofyc/argparse.h"
#include "orlp/ed25519.h"

static int cmd_verify(int argc, const char **argv) {
  const char *keyFile         = NULL;
  const char *message         = NULL;
  const char *messageFile     = NULL;
  const char *verifySignature = NULL;
  struct KeyPair *kp;
  FILE *fmessage;
  FILE *fsignature;
  int isValid;

  static const char *const usages[] = {
    "supercop verify -k keyfile -s signature [-m message | -M message-file]",
    NULL,
  };
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_STRING('k', "key-file", &keyFile, "Select key file to use for the operation", NULL, 0, 0),
    OPT_STRING('m', "message", &message, "Message to verify (defaults to stdin)", NULL, 0, 0),
    OPT_STRING('M', "message-file", &messageFile, "Message file to verify (defaults to stdin)", NULL, 0, 0),
    OPT_STRING('s', "signature", &verifySignature, "Signature to verify", NULL, 0, 0),
    OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nVerify a message signature", NULL);
  argparse_parse(&argparse, argc, argv);

  if (!keyFile) {
    fprintf(stderr, "Missing required argument: key-file\n\n");
    argparse_usage(&argparse);
    return 1;
  }

  if (!verifySignature) {
    fprintf(stderr, "Missing required argument: signature\n\n");
    argparse_usage(&argparse);
    return 1;
  }

  fmessage   = supercop_open_message(message, messageFile);
  fsignature = supercop_parse_signature(verifySignature);

  long message_len = fremaining(fmessage);
  const unsigned char *msg = calloc(1, message_len);
  fread((void *)msg, 1, message_len, fmessage);

  long signature_len = fremaining(fsignature);
  const unsigned char *sig = calloc(1, signature_len);
  fread((void *)sig, 1, signature_len, fsignature);
  if (signature_len != 64) {
    fprintf(stderr, "Invalid signature!!\n");
    free((void *)sig);
    free((void *)msg);
    fclose(fsignature);
    return 1;
  }

  kp = readKeyFile(keyFile);
  if (!kp || !kp->public_key) {
    fprintf(stderr, "Could not decode key file: unknown or invalid format\n");
    free((void *)sig);
    free((void *)msg);
    fclose(fsignature);
    if (kp) keypair_free(kp);
    return 1;
  }
  isValid = ed25519_verify(sig, msg, message_len, kp->public_key);

  free((void *)sig);
  free((void *)msg);
  fclose(fsignature);
  keypair_free(kp);

  if (isValid) {
    fprintf(stdout, "OK\n");
    return 0;
  } else {
    fprintf(stdout, "FAIL\n");
    return 1;
  }
}

void __attribute__((constructor)) cmd_verify_setup(void) {
  struct cmd_struct *cmd = calloc(1, sizeof(struct cmd_struct));
  if (!cmd) {
    fprintf(stderr, "Failed to allocate memory for verify command\n");
    return;
  }
  cmd->next                        = commands;
  cmd->fn                          = cmd_verify;
  static const char *verify_names[] = {"verify", NULL};
  cmd->name                        = verify_names;
  cmd->display                     = "verify";
  cmd->description                 = "Verify a message signature";
  cmd->help_text =
      "supercop verify - Verify a message signature\n"
      "\n"
      "Usage:\n"
      "  supercop verify -k keyfile -s signature [-m message | -M message-file]\n"
      "\n"
      "Options:\n"
      "  -k, --key-file <path>       Select key file to use for the operation\n"
      "  -m, --message <message>     Message to verify (defaults to stdin)\n"
      "  -M, --message-file <path>   Message file to verify (defaults to stdin)\n"
      "  -s, --signature <hex>       Signature to verify the message against\n";
  commands = cmd;
}

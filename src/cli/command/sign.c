#include <stdio.h>
#include <stdlib.h>

#include "cli/command.h"
#include "cli/common.h"
#include "cofyc/argparse.h"
#include "orlp/ed25519.h"

static int cmd_sign(int argc, const char **argv) {
  const char *keyFile     = NULL;
  const char *message     = NULL;
  const char *messageFile = NULL;
  struct KeyPair *kp;
  FILE *fmessage;
  int i, result;

  static const char *const usages[] = {
    "supercop sign -k keyfile [-m message | -M message-file]",
    NULL,
  };
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_STRING('k', "key-file", &keyFile, "Select key file to use for the operation", NULL, 0, 0),
    OPT_STRING('m', "message", &message, "Message to sign (defaults to stdin)", NULL, 0, 0),
    OPT_STRING('M', "message-file", &messageFile, "Message file to sign (defaults to stdin)", NULL, 0, 0),
    OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nSign a message", NULL);
  argparse_parse(&argparse, argc, argv);

  if (!keyFile) {
    fprintf(stderr, "Missing required argument: key-file\n\n");
    argparse_usage(&argparse);
    return 1;
  }

  fmessage = supercop_open_message(message, messageFile);
  kp = readKeyFile(keyFile);
  if (!kp || !kp->public_key) {
    fprintf(stderr, "Could not decode key file: unknown or invalid format\n");
    if (kp) keypair_free(kp);
    return 1;
  }
  if (!kp->private_key) {
    fprintf(stderr, "Key file contains no private key: signing requires a full key\n");
    keypair_free(kp);
    return 1;
  }

  long message_len = fremaining(fmessage);
  const unsigned char *msg = calloc(1, message_len);
  fread((void *)msg, 1, message_len, fmessage);

  unsigned char *signature = calloc(1, 64);
  ed25519_sign(signature, msg, message_len, kp->public_key, kp->private_key);

  for(i=0;i<64;i++) fprintf(stdout, "%02x", signature[i]);
  fprintf(stdout, "\n");

  free(signature);
  free((void *)msg);
  keypair_free(kp);
  result = 0;
  return result;
}

void __attribute__((constructor)) cmd_sign_setup(void) {
  struct cmd_struct *cmd = calloc(1, sizeof(struct cmd_struct));
  if (!cmd) {
    fprintf(stderr, "Failed to allocate memory for sign command\n");
    return;
  }
  cmd->next                      = commands;
  cmd->fn                        = cmd_sign;
  static const char *sign_names[] = {"sign", NULL};
  cmd->name                      = sign_names;
  cmd->display                   = "sign";
  cmd->description               = "Sign a message";
  cmd->help_text =
      "supercop sign - Sign a message\n"
      "\n"
      "Usage:\n"
      "  supercop sign -k keyfile [-m message | -M message-file]\n"
      "\n"
      "Options:\n"
      "  -k, --key-file <path>       Select key file to use for the operation\n"
      "  -m, --message <message>     Message to sign (defaults to stdin)\n"
      "  -M, --message-file <path>   Message file to sign (defaults to stdin)\n";
  commands = cmd;
}

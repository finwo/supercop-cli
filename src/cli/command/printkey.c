#include <stdio.h>
#include <stdlib.h>

#include "cli/command.h"
#include "cli/common.h"
#include "cofyc/argparse.h"

static int cmd_printkey(int argc, const char **argv) {
  const char *keyFile = NULL;
  struct KeyPair *kp;
  int i, result;

  static const char *const usages[] = {
    "supercop printkey -k keyfile",
    NULL,
  };
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_STRING('k', "key-file", &keyFile, "Select key file to use for the operation", NULL, 0, 0),
    OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nPrint contents of key file", NULL);
  argparse_parse(&argparse, argc, argv);

  if (!keyFile) {
    fprintf(stderr, "Missing required argument: key-file\n\n");
    argparse_usage(&argparse);
    return 1;
  }

  kp = readKeyFile(keyFile);
  if (!kp || !kp->public_key) {
    fprintf(stderr, "Could not decode key file: unknown or invalid format\n");
    if (kp) keypair_free(kp);
    return 1;
  }

  fprintf(stdout, "public-key: ");
  for(i=0;i<32;i++) fprintf(stdout, "%02x", kp->public_key[i]);
  if (kp->private_key) {
    fprintf(stdout, "\nprivate-key: ");
    for(i=0;i<64;i++) fprintf(stdout, "%02x", kp->private_key[i]);
  } else {
    fprintf(stdout, "\nprivate-key: (no private key in file)");
  }
  fprintf(stdout, "\n");

  result = 0;
  keypair_free(kp);
  return result;
}

void __attribute__((constructor)) cmd_printkey_setup(void) {
  struct cmd_struct *cmd = calloc(1, sizeof(struct cmd_struct));
  if (!cmd) {
    fprintf(stderr, "Failed to allocate memory for printkey command\n");
    return;
  }
  cmd->next                          = commands;
  cmd->fn                            = cmd_printkey;
  static const char *printkey_names[] = {"printkey", NULL};
  cmd->name                          = printkey_names;
  cmd->display                       = "printkey";
  cmd->description                   = "Print contents of key file";
  cmd->help_text =
      "supercop printkey - Print contents of key file\n"
      "\n"
      "Usage:\n"
      "  supercop printkey -k keyfile\n"
      "\n"
      "Options:\n"
      "  -k, --key-file <path>  Select key file to use for the operation\n";
  commands = cmd;
}

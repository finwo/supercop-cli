#include <stdio.h>
#include <stdlib.h>

#include "cli/command.h"
#include "cli/common.h"
#include "cofyc/argparse.h"

static int cmd_printkey(int argc, const char **argv) {
  const char *keyFile = NULL;
  const char *format  = "hdr";
  const char *outFile = NULL;
  int pubOnly = 0;
  struct KeyPair *kp;
  struct Format *fmt;
  struct KeyPair out;
  FILE *fout = stdout;
  size_t encoded_length;
  char *encoded;
  int result;

  static const char *const usages[] = {
    "supercop printkey [options]",
    NULL,
  };
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_STRING('k', "key-file", &keyFile, "Select key file to use for the operation", NULL, 0, 0),
    OPT_STRING('f', "format", &format, "Output format (0x00, asc, hdr, defaults to hdr)", NULL, 0, 0),
    OPT_STRING('o', "out", &outFile, "Write output to file instead of stdout", NULL, 0, 0),
    OPT_BOOLEAN(0, "public-only", &pubOnly, "Output only the public half of the key", NULL, 0, 0),
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

  fmt = supercop_find_format(format);
  if (!fmt) {
    fprintf(stderr, "Unknown format: %s\n", format);
    keypair_free(kp);
    return 1;
  }

  if (outFile) {
    fout = fopen(outFile, "w+");
    if (!fout) {
      fprintf(stderr, "Could not open output file\n");
      keypair_free(kp);
      return 1;
    }
  }

  // Shallow copy: encoders never take ownership, priv stripped on request
  out = *kp;
  if (pubOnly) out.private_key = NULL;

  encoded = fmt->encode(&out, &encoded_length);
  if (!encoded) {
    fprintf(stderr, "Error while encoding key\n");
    if (outFile && fout) fclose(fout);
    keypair_free(kp);
    return 1;
  }
  fwrite(encoded, 1, encoded_length, fout);
  if (encoded) free(encoded);

  result = 0;
  if (outFile && fout) fclose(fout);
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
      "  supercop printkey [options]\n"
      "\n"
      "Options:\n"
      "  -k, --key-file <path>  Select key file to use for the operation\n"
      "  -f, --format <name>    Output format: 0x00, asc, hdr (default hdr)\n"
      "  -o, --out <path>       Write output to file instead of stdout\n"
      "      --public-only      Output only the public half of the key\n";
  commands = cmd;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli/command.h"
#include "cli/common.h"
#include "cofyc/argparse.h"
#include "orlp/ed25519.h"

static int cmd_generate(int argc, const char **argv) {
  const char *keyFile = NULL;
  const char *format  = "0x00";
  FILE *fout = stdout;
  struct Format *fmt;
  struct KeyPair out;
  int result;

  static const char *const usages[] = {
    "supercop generate [options]",
    NULL,
  };
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_STRING('k', "key-file", &keyFile, "Key file to write (defaults to stdout)", NULL, 0, 0),
    OPT_STRING('f', "format", &format, "Key format to write (0x00, asc, hdr)", NULL, 0, 0),
    OPT_END(),
  };

  struct argparse argparse;
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nGenerate a new ed25519 key", NULL);
  argparse_parse(&argparse, argc, argv);

  if (keyFile) {
    fout = fopen(keyFile, "w+");
    if (!fout) {
      fprintf(stderr, "Could not open output file\n");
      return 1;
    }
  }

  unsigned char seed[32];
  unsigned char public_key[32];
  unsigned char private_key[64];

  if (ed25519_create_seed(seed)) {
    fprintf(stderr, "Error while generating seed\n");
    if (keyFile) fclose(fout);
    return 1;
  }

  ed25519_create_keypair(public_key, private_key, seed);
  out.public_key  = public_key;
  out.private_key = private_key;

  // Encode in the requested format (default keeps historical behavior)
  fmt = supercop_find_format(format);
  if (!fmt) {
    fprintf(stderr, "Unknown format: %s\n", format);
    if (keyFile) fclose(fout);
    return 1;
  }
  size_t encoded_length;
  char *encoded = fmt->encode(&out, &encoded_length);
  if (!encoded) {
    fprintf(stderr, "Error while encoding key\n");
    if (keyFile) fclose(fout);
    return 1;
  }
  fwrite(encoded, 1, encoded_length, fout);
  free(encoded);

  result = 0;
  if (keyFile) {
    fclose(fout);
  }

  return result;
}

void __attribute__((constructor)) cmd_generate_setup(void) {
  struct cmd_struct *cmd = calloc(1, sizeof(struct cmd_struct));
  if (!cmd) {
    fprintf(stderr, "Failed to allocate memory for generate command\n");
    return;
  }
  cmd->next                          = commands;
  cmd->fn                            = cmd_generate;
  static const char *generate_names[] = {"generate", NULL};
  cmd->name                          = generate_names;
  cmd->display                       = "generate";
  cmd->description                   = "Generate new key";
  cmd->help_text =
      "supercop generate - Generate new key\n"
      "\n"
      "Usage:\n"
      "  supercop generate [options]\n"
      "\n"
      "Options:\n"
      "  -k, --key-file <path>  Key file to write (defaults to stdout)\n"
      "  -f, --format <name>    Key format to write: 0x00, asc, hdr (default 0x00)\n";
  commands = cmd;
}

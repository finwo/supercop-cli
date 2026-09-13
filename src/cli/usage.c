#include "usage.h"

#include <stdio.h>
#include <stdlib.h>

#include "cofyc/argparse.h"
#include "cli/command.h"

static int opt_version = 0;

static const char *const usages[] = {
  "supercop [global options] <command> [command options]",
  "supercop list-commands",
  NULL,
};

static struct argparse_option options[] = {
  OPT_GROUP("Global options:"),
  OPT_BOOLEAN('h', "help", NULL, "Show this help message and exit", argparse_help_cb, 0, OPT_NONEG),
  OPT_BOOLEAN('V', "version", &opt_version, "Show version information and exit", NULL, 0, OPT_NONEG),
  OPT_END(),
};

// argparse prints the epilog at the end of its usage output, which is the only
// way to get our command listing into the built-in --help handler
static char *build_epilog(void) {
  char  *buffer = NULL;
  size_t size   = 0;
  FILE  *out    = open_memstream(&buffer, &size);
  if (!out) {
    return NULL;
  }

  struct cmd_struct *cmd = commands;

  // The %-17s aligns our descriptions with the ones argparse prints
  fprintf(out, "\nCommands:\n");
  while (cmd) {
    fprintf(out, "    %-17s %s\n", cmd->display ? cmd->display : cmd->name[0],
            cmd->description ? cmd->description : "");
    cmd = cmd->next;
  }

  fclose(out);

  // argparse adds a newline of its own
  if (size && buffer[size - 1] == '\n') {
    buffer[size - 1] = '\0';
  }

  return buffer;
}

int supercop_global_options(int argc, const char **argv, int *show_version) {
  struct argparse argparse;
  char           *epilog = build_epilog();

  opt_version = 0;
  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nMinimalistic program to generate ed25519 keys and verify/sign messages", epilog);
  argc = argparse_parse(&argparse, argc, argv);
  free(epilog);

  if (show_version) {
    *show_version = opt_version;
  }

  return argc;
}

void supercop_print_global_usage(void) {
  struct argparse argparse;
  char           *epilog = build_epilog();

  argparse_init(&argparse, options, usages, ARGPARSE_STOP_AT_NON_OPTION);
  argparse_describe(&argparse, "\nMinimalistic program to generate ed25519 keys and verify/sign messages", epilog);
  argparse_usage(&argparse);
  free(epilog);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "cli/command.h"
#include "cli/common.h"
#include "cli/usage.h"
#include "fmt/common.h"

struct Format *supercop_formats = NULL;
struct cmd_struct *commands = NULL;

int main(int argc, const char **argv) {
  int show_version = 0;

  argc = supercop_global_options(argc, argv, &show_version);

  /* --version is nothing but another way of calling the version command */
  if (show_version) {
    static const char *version_argv[] = {"version", NULL};
    argv                              = version_argv;
    argc                              = 1;
  }

  if (argc < 1) {
    supercop_print_global_usage();
    return 1;
  }

  /* Try to run command with args provided. */
  struct cmd_struct *cmd = commands;
  while (cmd) {
    const char **name = cmd->name;
    while (*name) {
      if (!strcmp(*name, argv[0])) {
        goto found;
      }
      name++;
    }
    cmd = cmd->next;
  }
found:

  if (cmd) {
    return cmd->fn(argc, argv);
  } else {
    fprintf(stderr, "Unknown command: %s\n", argv[0]);
    return 1;
  }

  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

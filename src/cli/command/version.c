#include <stdio.h>
#include <stdlib.h>

#include "cli/command.h"
#include "cli/common.h"

static int cmd_version(int argc, const char **argv) {
  (void)argc;
  (void)argv;
  print_version();
  return 0;
}

void __attribute__((constructor)) cmd_version_setup(void) {
  struct cmd_struct *cmd = calloc(1, sizeof(struct cmd_struct));
  if (!cmd) {
    fprintf(stderr, "Failed to allocate memory for version command\n");
    return;
  }
  cmd->next                         = commands;
  cmd->fn                           = cmd_version;
  static const char *version_names[] = {"version", NULL};
  cmd->name                         = version_names;
  cmd->display                      = "version";
  cmd->description                  = "Show version number and exit";
  cmd->help_text =
      "supercop version - Show version number and exit\n"
      "\n"
      "Usage:\n"
      "  supercop version\n"
      "\n"
      "Description:\n"
      "  Print the version of supercop and exit.\n"
      "\n"
      "  The global --version flag is dispatched to this command, so both\n"
      "  produce the exact same output.\n"
      "\n"
      "Examples:\n"
      "  supercop version           # Show the version\n"
      "  supercop --version         # Same, as a global flag\n"
      "  supercop -V                # Same, short form\n";
  commands = cmd;
}

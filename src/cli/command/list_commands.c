#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli/command.h"

static int cmd_list_commands(int argc, const char **argv) {
  (void)argc;
  (void)argv;

  // Find the longest command name for aligned output
  size_t width = 0;
  struct cmd_struct *cmd = commands;
  while (cmd) {
    size_t len = strlen(cmd->display ? cmd->display : cmd->name[0]);
    if (len > width) width = len;
    cmd = cmd->next;
  }

  cmd = commands;
  while (cmd) {
    printf("  %-*s  %s\n", (int)width,
           cmd->display ? cmd->display : cmd->name[0],
           cmd->description ? cmd->description : "");
    cmd = cmd->next;
  }

  return 0;
}

void __attribute__((constructor)) cmd_list_commands_setup(void) {
  struct cmd_struct *cmd = calloc(1, sizeof(struct cmd_struct));
  if (!cmd) {
    fprintf(stderr, "Failed to allocate memory for list-commands command\n");
    return;
  }
  cmd->next                               = commands;
  cmd->fn                                 = cmd_list_commands;
  static const char *list_commands_names[] = {"list-commands", NULL};
  cmd->name                               = list_commands_names;
  cmd->display                            = "list-commands";
  cmd->description                        = "List available commands";
  cmd->help_text =
      "supercop list-commands - List available commands\n"
      "\n"
      "Usage:\n"
      "  supercop list-commands\n";
  commands = cmd;
}

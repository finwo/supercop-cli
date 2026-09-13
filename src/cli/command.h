#ifndef __SUPERCOP_CLI_COMMAND_H__
#define __SUPERCOP_CLI_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

struct cmd_struct {
  void        *next;
  const char **name;
  const char  *display;
  const char  *description;
  const char  *help_text;
  int (*fn)(int, const char **);
};

extern struct cmd_struct *commands;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_CLI_COMMAND_H__

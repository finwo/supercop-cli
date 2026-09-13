#ifndef __SUPERCOP_CLI_USAGE_H__
#define __SUPERCOP_CLI_USAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

// Parse the global options, returns the remaining argument count
// Sets show_version when -V/--version was given, handles --help by itself
int supercop_global_options(int argc, const char **argv, int *show_version);

// Print the global usage: options, commands and help topics
void supercop_print_global_usage(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_CLI_USAGE_H__

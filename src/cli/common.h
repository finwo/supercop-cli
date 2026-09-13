#ifndef __SUPERCOP_CLI_COMMON_H__
#define __SUPERCOP_CLI_COMMON_H__

#include <stdio.h>

#include "fmt/common.h"
#include "keypair/keypair.h"

#ifdef __cplusplus
extern "C" {
#endif

// Intentionally minimal: this is a short-lived CLI, so it keeps the
// fprintf(stdout/stderr) + exit-code contract instead of adopting the
// rxi/log verbosity model used by the daemon projects (unosd, udphole).
// No config file, no environment variables: flags and stdin/files only.

extern struct Format *supercop_formats;

long fremaining(FILE *fd);
struct KeyPair *readKeyFile(const char *filename);

// Message source: -m string wins, then -M file, then stdin
FILE *supercop_open_message(const char *message, const char *messageFile);

// Parse a hex signature string into a rewinded tmpfile (must be 64 bytes)
FILE *supercop_parse_signature(const char *hex);

// Single implementation shared by the --version flag and version subcommand
void print_version(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_CLI_COMMON_H__

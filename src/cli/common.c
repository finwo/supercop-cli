#include "cli/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef VERSION
#define VERSION "n/a"
#endif

long fremaining(FILE *fd) {
  long current = ftell(fd);
  fseek(fd, 0, SEEK_END);
  long end = ftell(fd);
  fseek(fd, current, SEEK_SET);
  return end - current;
}

struct KeyPair *readKeyFile(const char *filename) {
  struct Format *fmt = supercop_formats;
  FILE *fd = fopen(filename, "r");
  unsigned char *buf;
  struct KeyPair *kp;

  if (!fd) {
    fprintf(stderr, "Could not open key file\n");
    exit(1);
  }

  // Read whole file
  long fsize = fremaining(fd);
  buf = calloc(1, fsize + 1);
  fread(buf, 1, fsize, fd);

  // Auto-detect format
  while(fmt) {
    if (!fmt->detect(buf, fsize)) {
      fmt = fmt->next;
      continue;
    }
    kp = fmt->decode(buf, fsize);
    if (!kp) {
      fmt = fmt->next;
      continue;
    }
    free(buf);
    fclose(fd);
    return kp;
  }

  free(buf);
  fclose(fd);
  return NULL;
}

FILE *supercop_open_message(const char *message, const char *messageFile) {
  FILE *fmessage = stdin;

  if (message) {
    fmessage = tmpfile();
    fwrite(message, 1, strlen(message), fmessage);
    fseek(fmessage, 0, SEEK_SET);
  }

  if (messageFile) {
    if (message) fclose(fmessage);
    fmessage = fopen(messageFile, "r");
    if (!fmessage) {
      fprintf(stderr, "Could not open message file\n");
      exit(1);
    }
  }

  return fmessage;
}

FILE *supercop_parse_signature(const char *hex) {
  FILE *fsignature = tmpfile();
  const char *pos = hex;
  unsigned char c;

  while(*pos) {
    sscanf(pos, "%2hhx", &c);
    fputc(c, fsignature);
    pos += 2;
  }
  fseek(fsignature, 0, SEEK_SET);

  return fsignature;
}

void print_version(void) {
  fprintf(stdout, "%s\n", VERSION);
}

#include <stddef.h>
#include <string.h>

#include "common.h"
#include "armor.h"

#ifdef __cplusplus
extern "C" {
#endif

char fmt_asc_detect(unsigned char *cipherdata, size_t len) {
  // Stream-friendly: either armor block may start on any line.
  // Claiming is cheap; decode validates and returns NULL on garbage,
  // letting readKeyFile move on to the next format.
  if (asc_find_block(cipherdata, len, ASC_LABEL_PRIVATE, NULL, NULL)) return 1;
  if (asc_find_block(cipherdata, len, ASC_LABEL_PUBLIC, NULL, NULL)) return 1;
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif

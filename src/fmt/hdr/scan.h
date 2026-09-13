// Internal line scanner shared by the hdr detect/decode implementations.
//
// A key line looks like:
//
//   [spaces/tabs] label [spaces/tabs] : [spaces/tabs] value [spaces/tabs]
//
// where label is public-key or private-key (case-insensitive). Matching is
// done against the length-bounded buffer, never assuming NUL-termination.

#ifndef __SUPERCOP_FMT_HDR_SCAN_H__
#define __SUPERCOP_FMT_HDR_SCAN_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// which: 1 = public-key, 2 = private-key
// Returns 1 and fills which/val/vallen when line holds a key label,
// 0 otherwise. val/vallen may point at an empty value; callers decide.
int hdr_match_label(const unsigned char *line, size_t linelen, int *which,
                    const unsigned char **val, size_t *vallen);

// Advance helpers for walking a length-bounded buffer line by line.
// Returns 1 while a line (without its '\n') is available.
int hdr_next_line(const unsigned char **pos, const unsigned char *end,
                  const unsigned char **line, size_t *linelen);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SUPERCOP_FMT_HDR_SCAN_H__

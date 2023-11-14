#ifndef FLAGS_H_
#define FLAGS_H_

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#define FLAG_CAT_b 0b00100000 /* number non-empty lines */
#define FLAG_CAT_E 0b00010000 /* display end-of-line characters as $ */
#define FLAG_CAT_n 0b00001000 /* number all output lines */
#define FLAG_CAT_s 0b00000100 /* squeeze multiple adjacent blank lines */
#define FLAG_CAT_T 0b00000010 /* display tabs as ^I */
#define FLAG_CAT_v 0b00000001 /* display non-printing characters */

/* Write flags bitmask to `flags`.
   Write index of the first non-option argument in `argv` to `first_arg_index`.
   Return 0 on success.
   Return `ERINVAL` defined in `errno.h` if encountered invalid option. */
int GetFlagsCat(const int argc, char *const *argv, int8_t *flags,
                int *first_arg_index);

/* Return non-zero if `flags` bitmask has a `flag`.
   Return zero otherwise. */
int8_t HasFlagCat(const int8_t flags, const int8_t flag);

/* pattern */
#define FLAG_GREP_e 0b0000001000000000

/* ignore uppercase vs. lowercase */
#define FLAG_GREP_i 0b0000000100000000

/* invert match */
#define FLAG_GREP_v 0b0000000010000000

/* output count of matching lines only */
#define FLAG_GREP_c 0b0000000001000000

/* output matching files only */
#define FLAG_GREP_l 0b0000000000100000

/* precede each matching line with a number */
#define FLAG_GREP_n 0b0000000000010000

/* output matching lines without preceding them by file names */
#define FLAG_GREP_h 0b0000000000001000

/* suppress error messages about nonexistent or unreadable files */
#define FLAG_GREP_s 0b0000000000000100

/* take regexes from a file */
#define FLAG_GREP_f 0b0000000000000010

/* output the matched parts of a matching line */
#define FLAG_GREP_o 0b0000000000000001

/* Write flags bitmask to `flags`.
   Write index of the first non-option argument in `argv` to `first_arg_index`.
   Return 0 on success.
   Return `EINVAL` or another error defined in `errno.h` that
   `getopt()`, `realloc()`, `malloc()`, `stat()`, `fopen()` and `getline()`
   functions may set `errno` to.  */
int GetFlagsGrep(const int argc, char *const *argv, int16_t *flags,
                 int *first_arg_index, char ***patterns,
                 size_t *patterns_length);

/* Return non-zero if `flags` bitmask has a `flag`.
   Return zero otherwise. */
int16_t HasFlagGrep(const int16_t flags, const int16_t flag);

#endif // FLAGS_H_

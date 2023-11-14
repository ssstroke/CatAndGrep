#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <stdio.h>

/* Add `new_string` to `strings` array and increase `string_length`.
   Return 0 on success.
   Return `ENOMEM` from `errno.h` on failure. */
int AddString(char *new_string, char ***strings, size_t *strings_length);

/* Traverse through `strings` and free memory if allocated. */
void FreeStrings(char ***strings, size_t strings_length);

/* Open file and set `file_pointer`. This is almost the same as
   `fopen(filename)` except that it produces an error if
   file with `filename` name is a directory.
   Return 0 on success.
   Return `EISDIR` or another error defined in `errno.h` that
   `stat()` and `fopen()` functions may set `errno` to. */
int OpenFileForReading(const char *filename, FILE **file_pointer);

#endif // UTILS_H_

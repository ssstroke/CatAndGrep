#include "utils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int AddString(char *new_string, char ***strings, size_t *strings_length) {
  int error = 0;

  char **strings_pointer = *strings;

  strings_pointer = (char **)realloc(strings_pointer,
                                     (*strings_length + 1) * sizeof(char **));
  if (errno != 0)
    error = errno;
  else {
    strings_pointer[*strings_length] =
        (char *)malloc(strlen(new_string) * sizeof(char *));
    if (errno != 0)
      error = errno;
    else {
      strcpy(strings_pointer[*strings_length], new_string);
      *strings = strings_pointer;
      ++(*strings_length);
    }
  }

  return error;
}

void FreeStrings(char ***strings, size_t strings_length) {
  char **strings_pointer = *strings;

  if (strings_pointer != NULL)
    for (size_t i = 0; i < strings_length; ++i)
      if (strings_pointer[i] != NULL)
        free(strings_pointer[i]);
}

int OpenFileForReading(const char *filename, FILE **file_pointer) {
  int error = 0;

  struct stat file_stat;

  if (stat(filename, &file_stat) == -1)
    error = errno;
  else if (S_ISDIR(file_stat.st_mode))
    error = EISDIR;
  else if ((*file_pointer = fopen(filename, "r")) == NULL)
    error = errno;

  return error;
}

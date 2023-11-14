#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../common/flags.h"
#include "../common/utils.h"

int PrintFile(FILE *file_pointer, const int8_t flags);
void PrintCharacter(const char c, const int8_t flags);
void PrintNonPrintable(const char c);

int main(int argc, char **argv) {
  const char *program_name = argv[0];

  int first_arg_index;
  int8_t flags;

  if (GetFlagsCat(argc, argv, &flags, &first_arg_index) != 0) {
    fprintf(stderr, "Usage: %s [-beEnstTv]... [FILE]...\n", program_name);
    exit(EXIT_FAILURE);
  }

  int error = 0;

  if (first_arg_index == argc) {
    error = PrintFile(stdin, flags);
    if (error != 0)
      fprintf(stderr, "%s: %s\n", program_name, strerror(error));
    else if (fclose(stdin) != 0) {
      error = errno;
      fprintf(stderr, "%s: (standart input): %s\n", program_name,
              strerror(error));
    }
  } else
    for (int i = first_arg_index; i < argc; ++i) {
      const char *filename = argv[i];
      FILE *file_pointer = NULL;

      error = OpenFileForReading(filename, &file_pointer);
      if (error != 0)
        fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                strerror(error));
      else {
        error = PrintFile(file_pointer, flags);
        if (error != 0)
          fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                  strerror(error));
        else if (fclose(file_pointer) != 0) {
          error = errno;
          fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                  strerror(error));
        }
      }
    }

  return error;
}

int PrintFile(FILE *file_pointer, const int8_t flags) {
  int error = 0;

  uintmax_t line_number = 0;
  bool squeeze_line = false;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  // TODO: This is not really optimal since flag is set only
  //       once so this an unnecessary branch decision for
  //       every line we process.
  while ((read = getline(&line, &len, file_pointer)) != -1) {
    if (HasFlagCat(flags, FLAG_CAT_s)) {
      if (squeeze_line == true && line[0] == '\n')
        continue;
      else
        squeeze_line = false;

      if (line[0] == '\n')
        squeeze_line = true;
    }

    if (HasFlagCat(flags, FLAG_CAT_n) ||
        (HasFlagCat(flags, FLAG_CAT_b) && line[0] != '\n'))
      printf("%6lu\t", ++line_number);

    for (ssize_t i = 0; i < read; ++i)
      PrintCharacter(line[i], flags);
  }

  if (errno != 0)
    error = errno;

  if (line != NULL)
    free(line);

  return error;
}

void PrintCharacter(const char c, const int8_t flags) {
  if (c == '\n' && HasFlagCat(flags, FLAG_CAT_E))
    printf("$\n");
  else if (c == '\t' && HasFlagCat(flags, FLAG_CAT_T))
    printf("^I");
  else if (c != '\n' && c != '\t' && HasFlagCat(flags, FLAG_CAT_v))
    PrintNonPrintable(c);
  else
    putchar(c);
}

void PrintNonPrintable(const char c) {
  if (c > 31 && c < 127)
    putchar(c);
  else if (c < 0) {
    printf("M-");
    PrintNonPrintable(c & 0b01111111);
  } else if (c < 32)
    printf("^%c", c + 64);
  else if (c == 127)
    printf("^?");
}

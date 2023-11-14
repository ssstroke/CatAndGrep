#include "flags.h"

#include <errno.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int GetFlagsCat(const int argc, char *const *argv, int8_t *flags,
                int *first_arg_index) {
  *flags = 0;

  int error = 0;

  struct option long_options[] = {{"number-non-blank", no_argument, 0, 'b'},
                                  {"show-ends", no_argument, 0, 'E'},
                                  {"number", no_argument, 0, 'n'},
                                  {"squeeze-blank", no_argument, 0, 's'},
                                  {"show-tabs", no_argument, 0, 'T'},
                                  {"show-nonprinting", no_argument, 0, 'v'}};
  int opt;

  while ((opt = getopt_long(argc, argv, "beEnstTv", long_options, 0)) != -1 &&
         error == 0) {
    switch (opt) {
    case 'b':
      *flags |= FLAG_CAT_b;
      break;

    case 'e':
      *flags |= FLAG_CAT_E | FLAG_CAT_v;
      break;
    case 'E':
      *flags |= FLAG_CAT_E;
      break;

    case 'n':
      *flags |= FLAG_CAT_n;
      break;
    case 's':
      *flags |= FLAG_CAT_s;
      break;

    case 't':
      *flags |= FLAG_CAT_T | FLAG_CAT_v;
      break;
    case 'T':
      *flags |= FLAG_CAT_T;
      break;

    case 'v':
      *flags |= FLAG_CAT_v;
      break;

    default:
      error = EINVAL;
      break;
    }
  }

  if (*flags & FLAG_CAT_b)
    *flags &= ~FLAG_CAT_n;
  *first_arg_index = optind;

  return error;
}

int8_t HasFlagCat(const int8_t flags, const int8_t flag) {
  return flags & flag;
}

int GetFlagsGrep(const int argc, char *const *argv, int16_t *flags,
                 int *first_arg_index, char ***patterns,
                 size_t *patterns_length) {
  *flags = 0;
  *patterns_length = 0;

  int error = 0;

  int opt;

  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && error == 0) {
    switch (opt) {
    case 'e':
      *flags |= FLAG_GREP_e;
      error = AddString(optarg, patterns, patterns_length);
      break;

    case 'i':
      *flags |= FLAG_GREP_i;
      break;
    case 'v':
      *flags |= FLAG_GREP_v;
      break;
    case 'c':
      *flags |= FLAG_GREP_c;
      break;
    case 'l':
      *flags |= FLAG_GREP_l;
      break;
    case 'n':
      *flags |= FLAG_GREP_n;
      break;
    case 'h':
      *flags |= FLAG_GREP_h;
      break;
    case 's':
      *flags |= FLAG_GREP_s;
      break;

    case 'f': {
      *flags |= FLAG_GREP_f;

      FILE *file_pointer = NULL;

      if ((error = OpenFileForReading(optarg, &file_pointer)) == 0) {
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, file_pointer)) != -1 &&
               error == 0) {
          if (line[read - 1] == '\n')
            line[read - 1] = '\0';
          error = AddString(line, patterns, patterns_length);
        }

        if (line != NULL)
          free(line);
      }

      if (file_pointer != NULL)
        fclose(file_pointer);
    } break;

    case 'o':
      *flags |= FLAG_GREP_o;
      break;

    default:
      error = EINVAL;
      break;
    }
  }

  if (*flags & FLAG_GREP_l)
    *flags &= ~FLAG_GREP_c;

  *first_arg_index = optind;
  if (*patterns == NULL && *first_arg_index == argc)
    error = EINVAL;

  return error;
}

int16_t HasFlagGrep(const int16_t flags, const int16_t flag) {
  return flags & flag;
}

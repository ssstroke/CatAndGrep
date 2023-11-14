#include <errno.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/flags.h"
#include "../common/utils.h"

int CompileRegs(regex_t **regs, const char **patterns,
                const size_t patterns_length, const bool case_sensitive,
                const bool ignore_subregs);
int PrintFile(FILE *file_pointer, const char *filename, const regex_t *regs,
              const size_t regs_length, const int16_t flags,
              bool print_filenames);

int main(int argc, char **argv) {
  char *program_name = argv[0];

  if (argc == 1) {
    fprintf(stderr, "Usage: %s [-eivclnhsfo] PATTERNS [FILE]...\n",
            program_name);
    exit(EXIT_FAILURE);
  }

  int error = 0;

  int16_t flags = 0;
  int first_arg_index = 0;
  char **patterns = NULL;
  size_t patterns_length = 0;

  error = GetFlagsGrep(argc, argv, &flags, &first_arg_index, &patterns,
                       &patterns_length);
  if (error == 0 && patterns == NULL) {
    error = AddString(argv[first_arg_index], &patterns, &patterns_length);
    ++first_arg_index;
  }

  regex_t *regs = NULL;
  if (error == 0) {
    bool case_insensitive = (HasFlagGrep(flags, FLAG_GREP_i)) ? true : false;
    bool ignore_subregs = (HasFlagGrep(flags, FLAG_GREP_o)) ? false : true;
    error = CompileRegs(&regs, (const char **)patterns, patterns_length,
                        case_insensitive, ignore_subregs);
  }

  if (error != 0)
    fprintf(stderr, "%s: %s\n", program_name, strerror(error));
  else if (first_arg_index == argc) {
    error = PrintFile(stdin, "(standart input)", regs, patterns_length, flags,
                      false);
    if (error != 0)
      fprintf(stderr, "%s: (standart input): %s\n", program_name,
              strerror(error));
    else if (fclose(stdin) != 0) {
      error = errno;
      fprintf(stderr, "%s: (standart input): %s\n", program_name,
              strerror(error));
    }
  } else {
    bool print_filenames = (argc - first_arg_index > 1) ? true : false;
    for (int i = first_arg_index; i < argc; ++i) {
      const char *filename = argv[i];
      FILE *file_pointer = NULL;

      error = OpenFileForReading(filename, &file_pointer);
      if (error != 0) {
        if (HasFlagGrep(flags, FLAG_GREP_s) == 0)
          fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                  strerror(error));
      } else {
        if (HasFlagGrep(flags, FLAG_GREP_h) && print_filenames == true)
          print_filenames = false;

        error = PrintFile(file_pointer, filename, regs, patterns_length, flags,
                          print_filenames);
        if (error != 0)
          fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                  strerror(error));
      }

      if (file_pointer != NULL && fclose(file_pointer) != 0) {
        error = errno;
        fprintf(stderr, "%s: %s: %s\n", program_name, filename,
                strerror(error));
      }
    }
  }

  if (patterns != NULL) {
    // TODO: Does it really need to be a separate function?
    FreeStrings(&patterns, patterns_length);
    free(patterns);
  }

  if (regs != NULL) {
    for (size_t i = 0; i < patterns_length; ++i)
      regfree(&(regs[i]));
    free(regs);
  }

  return error;
}

int CompileRegs(regex_t **regs, const char **patterns,
                const size_t patterns_length, const bool case_insensitive,
                const bool ignore_subregs) {
  int error = 0;

  int cflags = (case_insensitive == true) ? REG_ICASE : 0;
  cflags = (ignore_subregs == true) ? cflags | REG_NOSUB : cflags;

  regex_t *regs_pointer = malloc(patterns_length * sizeof(regex_t));
  if (errno != 0)
    error = errno;
  else
    for (size_t i = 0; i < patterns_length && error == 0; ++i)
      error = regcomp(&(regs_pointer[i]), patterns[i], cflags);

  *regs = regs_pointer;

  return error;
}

int PrintFile(FILE *file_pointer, const char *filename, const regex_t *regs,
              const size_t regs_length, const int16_t flags,
              bool print_filenames) {
  int error = 0;

  size_t line_number = 1;
  size_t match_count = 0;
  bool stop_reading_file = false;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file_pointer)) != -1 &&
         stop_reading_file == false) {
    regmatch_t pmatch[1];
    bool has_match = false;

    for (size_t i = 0; i < regs_length && has_match == false; ++i)
      has_match = (regexec(&(regs[i]), line, 1, pmatch, 0)) == 0 ? true : false;
    has_match = (HasFlagGrep(flags, FLAG_GREP_v)) ? !has_match : has_match;

    if (has_match) {
      if (HasFlagGrep(flags, FLAG_GREP_c))
        ++match_count;

      else if (HasFlagGrep(flags, FLAG_GREP_l))
        stop_reading_file = true;

      else if (HasFlagGrep(flags, FLAG_GREP_o)) {
        if (HasFlagGrep(flags, FLAG_GREP_v) == 0) {
          for (size_t i = 0; i < regs_length; ++i) {
            char *line_pointer = line;
            while (regexec(&(regs[i]), line_pointer, 1, pmatch, 0) == 0) {
              if (print_filenames)
                printf("%s:", filename);
              if (HasFlagGrep(flags, FLAG_GREP_n))
                printf("%lu:", line_number);

              printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                     line_pointer + pmatch[0].rm_so);
              line_pointer += pmatch[0].rm_eo;
            }
          }
        }
      }

      else {
        if (print_filenames)
          printf("%s:", filename);
        if (HasFlagGrep(flags, FLAG_GREP_n))
          printf("%lu:", line_number);

        printf("%s", line);

        if (line[read - 1] != '\n')
          putchar('\n');
      }
    }

    ++line_number;
  }

  if (HasFlagGrep(flags, FLAG_GREP_c)) {
    if (print_filenames)
      printf("%s:", filename);
    printf("%lu\n", match_count);
  }

  if (stop_reading_file == true)
    printf("%s\n", filename);

  if (line != NULL)
    free(line);

  if (errno != 0)
    error = errno;

  return error;
}

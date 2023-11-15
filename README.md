# Cat & Grep

## Introduction

Implementation of `cat` and `grep` utilities using the C programming language.

This is a project I did while attending programming school. Main goal of this project is to familirize myself with the C programming language and structured programming. These implementations are far worse than those provided by [GNU](https://www.gnu.org/software/grep/) and [BSD](https://cgit.freebsd.org/src/tree/usr.bin/grep/grep.c) in terms of, at least, performance. All of the code is pretty much straightforward and does not buffer the output, for example, that may be a huge boost to pefromance. I might try to do that later. Though implementation of `cat` found in this repository works better with non-ASCII characters compared to implementation provided by [BSD](https://cgit.freebsd.org/src/tree/usr.bin/grep/grep.c) (which is used in macOS, for example).

## Build

```
$ cd src/cat
$ make
```

```
$ cd src/grep
$ make
```

## Options

### `cat` options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display LFD as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display TAB as ^I  |
| 6 | -v | use ^ and M- notation, except for LFD and TAB  |

### `grep` options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e <_pattern_> | supply _pattern_ |
| 2 | -i | ignore uppercase vs. lowercase  |
| 3 | -v | invert match |
| 4 | -c | output count of matching lines only |
| 5 | -l | output matching files only  |
| 6 | -n | precede each matching line with a line number |
| 7 | -h | output matching lines without preceding them by file names |
| 8 | -s | suppress error messages about nonexistent or unreadable files |
| 9 | -f <_file_> | take patterns from a _file_ |
| 10 | -o | output the matched parts of a matching line |

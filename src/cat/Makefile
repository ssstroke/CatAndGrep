CC     := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L

DIR_COMMON := ../common

cat: cat.o $(DIR_COMMON)/flags.o $(DIR_COMMON)/utils.o
	$(CC) $(CFLAGS) $^ -o $@

cat.o: cat.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(DIR_COMMON)/flags.o: $(DIR_COMMON)/flags.c $(DIR_COMMON)/flags.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_COMMON)/utils.o: $(DIR_COMMON)/utils.c $(DIR_COMMON)/utils.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o cat $(DIR_COMMON)/*.o
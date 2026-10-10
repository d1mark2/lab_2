CC = clang
CFLAGS = -Wall -Werror
SOURCES := $(wildcard *.c)
OBJ = $(SOURCES:.c=.o)

main: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	rm -f $(OBJ)


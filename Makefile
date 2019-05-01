CC = gcc
OPTS = -g -Wall -Werror -std=c99
SRCS = $(wildcard *.c)
EXECS = $(SRCS:.c=)

all: $(EXECS)

%: %.c ${INCS}
	$(CC) $(OPTS) $< -o $@

.PHONY: clean
clean:
	rm $(EXECS)
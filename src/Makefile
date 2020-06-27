CC=gcc
CFLAGS=-pthread

SRC = server_code.c client_code.c
BIN = $(patsubst %.c,%,$(SRC))

all: $(BIN)

clean:
	rm -f $(BIN)

.PHONY: all clean

CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic
LIBS=-lraylib


EMFLAGS=-Os -Wall -I. -I./raylib/src -L. -L./raylib/src -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB --shell-file shell.html
NAME=game


all: main.o snake.o
	@$(CC) $(CFLAGS) $(LIBS) $^ -o out
	@echo CC $^

web: main.c snake.c ./raylib/src/libraylib.a
	emcc $(CFLAGS) $(EMFLAGS) $^ -o $(NAME).html

%.o: %.c Makefile %.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo CC $<

clean:
	rm *.o out  $(NAME).js $(NAME).html $(NAME).wasm

.PHONY: clean

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra  -Wno-unused-parameter -g
CTEST_FLAGS = -std=c99 -g
LDFLAGS = -lcriterion

SOURCES = src/main.c
# TEST_SOURCES = 

all: clox

clox: src/main.c $(SOURCES)
	@$(CC) $(CFLAGS) -o clox src/main.c $(SOURCES) -I. && echo Compiled!!!!

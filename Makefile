CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g
CTEST_FLAGS = -std=c99
LDFLAGS = -lcriterion

SOURCES = src/command_line_parser.c src/errors.c src/file_system.c
TEST_SOURCES = tests/command_line_parser_test.c 

all: genc

genc: src/main.c $(SOURCES)
	@$(CC) $(CFLAGS) -o genc src/main.c $(SOURCES) -I. && echo Compiled!!!!

run: genc
	@./genc

test: $(TEST_SOURCES) $(SOURCES)
	@$(CC) $(CTEST_FLAGS) -o genc_tests $(TEST_SOURCES) $(SOURCES) -I. $(LDFLAGS)
	@./genc_tests $(if $(ARGS),--filter "$(ARGS)")

clean:
	@rm -f test_runner genc

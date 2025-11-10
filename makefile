CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fsanitize=address
CLIBS = -lm

SOURCES = functions.c
TEST_SOURCES = tests.c functions.c
EXECUTABLE = laba
TEST_EXECUTABLE = tests

.PHONY: all test clean unit_test

all: $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	@$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE) $(CLIBS)

clean:
	@rm -f $(EXECUTABLE) $(TEST_EXECUTABLE)

unit_test: $(TEST_EXECUTABLE)
	@echo "=== Running unit tests ==="
	@./$(TEST_EXECUTABLE)
	@echo "=== Unit tests completed ==="

test: unit_test
	@echo ""
	@echo "=== All tests completed successfully! ==="
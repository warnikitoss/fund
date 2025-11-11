CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fsanitize=address
TARGET = main
TEST_TARGET = tests

.PHONY: all test clean

all: $(TARGET)

$(TARGET): main.c vector.h
	@$(CC) $(CFLAGS) main.c -o $(TARGET)

$(TEST_TARGET): tests.c vector.h
	@$(CC) $(CFLAGS) tests.c -o $(TEST_TARGET)

test: $(TEST_TARGET)
	@echo "=== Running unit tests ==="
	@./$(TEST_TARGET)
	@echo "=== Unit tests completed ==="

clean:
	@rm -f $(TARGET) $(TEST_TARGET)
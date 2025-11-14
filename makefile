CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fsanitize=address
CLIBS = -lm

SOURCES = functions.c main.c
TEST_SOURCES = functions.c tests.c
EXECUTABLE = program
TEST_EXECUTABLE = tests

.PHONY: all test clean unit_test integration_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(CLIBS)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	@$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE) $(CLIBS)

clean:
	@rm -f $(EXECUTABLE) $(TEST_EXECUTABLE) test_input.txt test_log.txt

unit_test: $(TEST_EXECUTABLE)
	@echo "=== Running unit tests ==="
	@./$(TEST_EXECUTABLE)
	@echo "=== Unit tests completed ==="
	@echo ""

integration_test: $(EXECUTABLE)
	@echo "=== Running integration tests ==="
	
	@echo "Test 1: Basic assignment and print"
	@echo "A=5" > test_input.txt
	@echo "print(A)" >> test_input.txt
	@./$(EXECUTABLE) test_input.txt test_log.txt > /dev/null 2>&1; \
	if [ $$? -eq 0 ]; then echo "SUCCESS: Basic assignment and print"; else echo "ERROR: Basic assignment and print failed"; fi
	
	@echo ""
	@echo "Test 2: Arithmetic operations"
	@echo "B=2+3" > test_input.txt
	@echo "C=10-4" >> test_input.txt
	@echo "D=6*7" >> test_input.txt
	@echo "E=20/5" >> test_input.txt
	@echo "print(B)" >> test_input.txt
	@./$(EXECUTABLE) test_input.txt test_log.txt > /dev/null 2>&1; \
	if [ $$? -eq 0 ]; then echo "SUCCESS: Arithmetic operations"; else echo "ERROR: Arithmetic operations failed"; fi
	
	@echo ""
	@echo "Test 3: Division by zero error"
	@echo "F=10/0" > test_input.txt
	@./$(EXECUTABLE) test_input.txt test_log.txt > /dev/null 2>&1; \
	exit_code=$$?; \
	if [ $$exit_code -eq 6 ]; then echo "SUCCESS: Division by zero detected"; else echo "ERROR: Division by zero not detected (got $$exit_code)"; fi
	
	@echo ""
	@echo "Test 4: Uninitialized variable error"
	@echo "print(Z)" > test_input.txt
	@./$(EXECUTABLE) test_input.txt test_log.txt > /dev/null 2>&1; \
	exit_code=$$?; \
	if [ $$exit_code -eq 7 ]; then echo "SUCCESS: Uninitialized variable detected"; else echo "ERROR: Uninitialized variable not detected (got $$exit_code)"; fi
	
	@echo ""
	@echo "Test 5: Complex expressions"
	@echo "X=2+3*4" > test_input.txt
	@echo "Y=10^2" >> test_input.txt
	@./$(EXECUTABLE) test_input.txt test_log.txt > /dev/null 2>&1; \
	if [ $$? -eq 0 ]; then echo "SUCCESS: Complex expressions"; else echo "ERROR: Complex expressions failed"; fi
	
	@echo ""
	@echo "Test 6: Invalid syntax"
	@echo "A=5+" > test_input.txt
	@./$(EXECUTABLE) test_input.txt test_log.txt > /dev/null 2>&1; \
	exit_code=$$?; \
	if [ $$exit_code -eq 4 ]; then echo "SUCCESS: Invalid syntax detected"; else echo "ERROR: Invalid syntax not detected (got $$exit_code)"; fi
	
	@echo "=== Integration tests completed ==="
	@rm -f test_input.txt test_log.txt

test: unit_test integration_test
	@echo ""
	@echo "=== All tests completed successfully! ==="
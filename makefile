CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fsanitize=address
CLIBS = -lm

SOURCES = heap.c mail_system.c main.c
TEST_SOURCES = heap.c mail_system.c tests.c
EXECUTABLE = mail_system
TEST_EXECUTABLE = tests

.PHONY: all test clean unit_test integration_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(CLIBS)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	@$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE) $(CLIBS)

clean:
	@rm -f $(EXECUTABLE) $(TEST_EXECUTABLE) test_log.txt letters_output.txt

unit_test: $(TEST_EXECUTABLE)
	@echo "=== Running unit tests ==="
	@./$(TEST_EXECUTABLE)
	@echo "=== Unit tests completed ==="
	@echo ""

integration_test: $(EXECUTABLE)
	@echo "=== Running integration tests ==="
	
	@echo "Test 1: Create mail system"
	@echo "8" | ./$(EXECUTABLE) mappings.txt test_log.txt > /dev/null 2>&1 && echo "SUCCESS: Mail system created" || echo "ERROR: Failed to create mail system"
	
	@echo ""
	@echo "Test 2: Add office"
	@printf "1\n100\n10\n2\n200\n300\n8\n" | ./$(EXECUTABLE) mappings.txt test_log.txt > /dev/null 2>&1 && echo "SUCCESS: Office added" || echo "ERROR: Failed to add office"
	
	@echo ""
	@echo "Test 3: Add letter"
	@printf "3\n0\n1\n100\n200\ntest_data\n8\n" | ./$(EXECUTABLE) mappings.txt test_log.txt > /dev/null 2>&1 && echo "SUCCESS: Letter added" || echo "ERROR: Failed to add letter"
	
	@echo ""
	@echo "Test 4: List letters"
	@printf "6\nletters_output.txt\n8\n" | ./$(EXECUTABLE) mappings.txt test_log.txt > /dev/null 2>&1 && [ -f "letters_output.txt" ] && echo "SUCCESS: Letters listed" || echo "ERROR: Failed to list letters"
	
	@echo ""
	@echo "Test 5: Transfer letters"
	@printf "7\n8\n" | ./$(EXECUTABLE) mappings.txt test_log.txt > /dev/null 2>&1 && echo "SUCCESS: Letters transferred" || echo "ERROR: Failed to transfer letters"
	
	@echo "=== Integration tests completed ==="

test: unit_test integration_test
	@echo ""
	@echo "=== All tests completed successfully! ==="

valgrind_test: $(EXECUTABLE)
	@echo "=== Running valgrind memory check ==="
	@echo "8" | valgrind --leak-check=full --track-origins=yes ./$(EXECUTABLE) mappings.txt test_log.txt 2>&1 | grep -E "ERROR SUMMARY|definitely lost"
	@echo "=== Valgrind check completed ==="

debug_test: $(TEST_EXECUTABLE)
	@echo "=== Running tests with debug output ==="
	@gdb -ex "run" -ex "bt" -ex "quit" --args ./$(TEST_EXECUTABLE)
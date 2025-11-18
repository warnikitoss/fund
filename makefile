CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wpedantic -fsanitize=address
CLIBS = -lm

SOURCES = functions.c main.c
TEST_SOURCES = functions.c tests.c
EXECUTABLE = residents
TEST_EXECUTABLE = tests

.PHONY: all test clean unit_test integration_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	@$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) $(CLIBS)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	@$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE) $(CLIBS)

clean:
	@rm -f $(EXECUTABLE) $(TEST_EXECUTABLE)

unit_test: $(TEST_EXECUTABLE)
	@echo "=== Running unit tests ==="
	@./$(TEST_EXECUTABLE)
	@echo "=== Unit tests completed ==="
	@echo ""

integration_test: $(EXECUTABLE) test_data.txt
	@echo "=== Running integration tests ==="
	
	@echo "Test 1: Program startup"
	@echo "0" | timeout 5 ./$(EXECUTABLE) 2>&1 | grep -q "Учет жителей" && echo "SUCCESS: Program started correctly" || echo "ERROR: Program failed to start"
	
	@echo ""
	@echo "Test 2: Add liver"
	@printf "3\n1\nTestov\nTest\nTestovich\n1\n1\n2000\nM\n5000\n0\n" | timeout 5 ./$(EXECUTABLE) 2>&1 | grep -q "Житель добавлен" && echo "SUCCESS: Liver added correctly" || echo "ERROR: Failed to add liver"
	
	@echo ""
	@echo "Test 3: Display all (empty list)"
	@printf "7\n0\n" | timeout 5 ./$(EXECUTABLE) 2>&1 | grep -q "Список пуст" && echo "SUCCESS: Empty list displayed correctly" || echo "ERROR: Display function failed"
	
	@echo ""
	@echo "Test 4: File operations - load from file"
	@printf "1\ntest_data.txt\n0\n" | timeout 5 ./$(EXECUTABLE) 2>&1 | grep -q "Загружено" && echo "SUCCESS: File load works" || (echo "ERROR: File load failed" && echo "Debug info:" && printf "1\ntest_data.txt\n0\n" | timeout 5 ./$(EXECUTABLE) 2>&1 | tail -5)
	
	@echo ""
	@echo "Test 5: Display all (after file load)"
	@printf "1\ntest_data.txt\n7\n0\n" | timeout 5 ./$(EXECUTABLE) 2>&1 | grep -q "Ivanov\|Petrova\|Sidorov" && echo "SUCCESS: Data loaded and displayed correctly" || echo "ERROR: Data not loaded correctly"
	
	@echo ""
	@echo "Test 6: Search liver by surname"
	@printf "1\ntest_data.txt\n6\n2\nIvanov\n0\n" | timeout 5 ./$(EXECUTABLE) 2>&1 | grep -q "Ivanov" && echo "SUCCESS: Search by surname works" || echo "ERROR: Search failed"
	
	@echo "=== Integration tests completed ==="

test_data.txt:
	@echo "Creating test data file..."
	@echo "1 Ivanov Ivan Ivanovich 15 5 1985 M 25000.50" > test_data.txt
	@echo "2 Petrova Anna Sergeevna 22 8 1990 W 30000.75" >> test_data.txt
	@echo "3 Sidorov Pavel Dmitrievich 10 12 1978 M 40000.00" >> test_data.txt
	@echo "Test data file created: test_data.txt"

test: test_data.txt unit_test integration_test
	@echo ""
	@echo "=== All tests completed! ==="

clean_data:
	@rm -f test_data.txt
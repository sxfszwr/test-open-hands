# Makefile for OverspeedWarning module
# Overspeed warning module build configuration

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lm

# Source files
SOURCES = overspeed_warning.c main_example.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = overspeed_warning_demo

# English source files
SOURCES_EN = overspeed_warning.c main_example_en.c
OBJECTS_EN = $(SOURCES_EN:.c=.o)
TARGET_EN = overspeed_warning_demo_en

# Unit test files
TEST_SOURCES = overspeed_warning.c unit_test.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_TARGET = unit_test

# English unit test files
TEST_EN_SOURCES = overspeed_warning.c unit_test_en.c
TEST_EN_OBJECTS = $(TEST_EN_SOURCES:.c=.o)
TEST_EN_TARGET = unit_test_en

# Header files
HEADERS = overspeed_warning.h

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Build English executable
$(TARGET_EN): $(OBJECTS_EN)
	$(CC) $(OBJECTS_EN) -o $(TARGET_EN) $(LDFLAGS)

# Build unit test
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS)

# Build English unit test
$(TEST_EN_TARGET): $(TEST_EN_OBJECTS)
	$(CC) $(TEST_EN_OBJECTS) -o $(TEST_EN_TARGET) $(LDFLAGS)

# Build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(OBJECTS_EN) $(TEST_OBJECTS) $(TEST_EN_OBJECTS) $(TARGET) $(TARGET_EN) $(TEST_TARGET) $(TEST_EN_TARGET)

# Run example (Chinese)
run: $(TARGET)
	./$(TARGET)

# Run example (English)
run-en: $(TARGET_EN)
	./$(TARGET_EN)

# Run unit tests (Chinese)
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Run unit tests (English)
test-en: $(TEST_EN_TARGET)
	./$(TEST_EN_TARGET)

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Uninstall (optional)
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# Show help information
help:
	@echo "Available make targets:"
	@echo "  all       - Build all files (default)"
	@echo "  clean     - Clean build artifacts"
	@echo "  run       - Build and run example program (Chinese)"
	@echo "  run-en    - Build and run example program (English)"
	@echo "  test      - Build and run unit tests (Chinese)"
	@echo "  test-en   - Build and run unit tests (English)"
	@echo "  install   - Install to system path"
	@echo "  uninstall - Uninstall from system path"
	@echo "  help      - Show this help information"

.PHONY: all clean run run-en test test-en install uninstall help
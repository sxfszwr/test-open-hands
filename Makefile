# Makefile for OverspeedWarning module
# Overspeed warning module build configuration

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lm

# Source files
SOURCES = overspeed_warning.c main_example.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = overspeed_warning_demo

# Unit test files
TEST_SOURCES = overspeed_warning.c unit_test.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_TARGET = unit_test

# Header files
HEADERS = overspeed_warning.h

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Build unit test
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS)

# Build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)

# Run example
run: $(TARGET)
	./$(TARGET)

# Run unit tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

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
	@echo "  run       - Build and run example program"
	@echo "  test      - Build and run unit tests"
	@echo "  install   - Install to system path"
	@echo "  uninstall - Uninstall from system path"
	@echo "  help      - Show this help information"

.PHONY: all clean run test install uninstall help
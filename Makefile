# Makefile for OverspeedWarning module
# 超速告警模块编译配置

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lm

# 源文件
SOURCES = overspeed_warning.c main_example.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = overspeed_warning_demo

# 单元测试
TEST_SOURCES = overspeed_warning.c unit_test.c
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_TARGET = unit_test

# 头文件
HEADERS = overspeed_warning.h

# 默认目标
all: $(TARGET)

# 编译可执行文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# 编译单元测试
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) -o $(TEST_TARGET) $(LDFLAGS)

# 编译目标文件
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译产物
clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGET)

# 运行示例
run: $(TARGET)
	./$(TARGET)

# 运行单元测试
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# 安装（可选）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# 卸载（可选）
uninstall:
	rm -f /usr/local/bin/$(TARGET)

# 显示帮助信息
help:
	@echo "可用的make目标:"
	@echo "  all       - 编译所有文件（默认）"
	@echo "  clean     - 清理编译产物"
	@echo "  run       - 编译并运行示例程序"
	@echo "  test      - 编译并运行单元测试"
	@echo "  install   - 安装到系统路径"
	@echo "  uninstall - 从系统路径卸载"
	@echo "  help      - 显示此帮助信息"

.PHONY: all clean run test install uninstall help
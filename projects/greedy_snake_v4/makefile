# 定义变量
# 匹配当前文件夹所有的*.c
SRCS := $(wildcard *.c)

# 将SRCS中的.c替换成.o，返回的是 main.o file.o slist.o function.o menu.o
OBJS := $(patsubst %.c, %.o, $(SRCS))
CC := gcc

# 可执行程序的名称
PROJECT_NAME := greedy_snake
TARGET := ./$(PROJECT_NAME)

# 规避目标与源文件同名，使用 .PHONY
.PHONY: all clean run

# 默认目标
all: $(PROJECT_NAME)

# 构建可执行文件
$(PROJECT_NAME): $(OBJS)
	$(CC) $^ -o $(TARGET) -lncurses
	
# 从.c生成.o的规则
%.o: %.c
	$(CC) -c $< -o $@
	
# 构建清除伪目标
clean:
	rm -f $(OBJS) $(TARGET)
	
# 构建运行伪目标
run: $(PROJECT_NAME)
	$(TARGET)
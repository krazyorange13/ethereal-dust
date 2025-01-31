WFLAGS = -Wall -Wextra
IFLAGS = -I src/ -I include/
LFLAGS = -L lib/ -lSDL3
MFLAGS = -O3
FLAGS = $(IFLAGS) $(LFLAGS) $(WFLAGS) $(MFLAGS)

CC = gcc-14
CC_FLAGS = $(FLAGS)

TARGET = out
BUILD_DIR = ./build

C_FILES = $(wildcard src/*.c)
O_FILES = $(C_FILES:%.c=$(BUILD_DIR)/%.o)
D_FILES = $(O_FILES:%.o=%.d)

$(TARGET): $(BUILD_DIR)/$(TARGET)


$(BUILD_DIR)/$(TARGET): $(O_FILES)
	@-mkdir -p $(@D)
	$(CC) $(LFLAGS) $(WFLAGS) $(MFLAGS) $^ -o $@

-include $(D_FILES)
$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(IFLAGS) $(WFLAGS) $(MFLAGS) -MMD -c $< -o $@

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/$(TARGET) $(O_FILES) $(D_FILES)

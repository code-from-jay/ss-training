TARGET_EXEC ?= skip_linker_test

CPP_COMPILER ?= clang++

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -g

compile: $(OBJS)

link: compile
	$(CPP_COMPILER) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(LDFLAGS)

# C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CPP_COMPILER) $(CPPFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

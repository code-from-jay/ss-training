TARGET_EXEC ?= integer_overflow

CPP_COMPILER ?= clang++

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -std=c++17

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CPP_COMPILER) $(OBJS) -o $@ $(LDFLAGS)

# C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CPP_COMPILER) $(CPPFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

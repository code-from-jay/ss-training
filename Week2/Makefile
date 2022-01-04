TARGET_EXEC ?= bankcli

CPP_COMPILER ?= clang++

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.cc)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -std=c++17
CPPFLAGS_TESTS := $(CPPFLAGS) -I. -DRUN_TESTS
LDFLAGS ?= -lprotobuf

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CPP_COMPILER) $(OBJS) -o $@ $(LDFLAGS)

# C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CPP_COMPILER) $(CPPFLAGS) -c $< -o $@

# TODO: Figure out how to not repeat this target
$(BUILD_DIR)/%.cc.o: %.cc
	mkdir -p $(dir $@)
	$(CPP_COMPILER) $(CPPFLAGS) -c $< -o $@


tests: CPPFLAGS=$(CPPFLAGS_TESTS)
tests: $(BUILD_DIR)/$(TARGET_EXEC)


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

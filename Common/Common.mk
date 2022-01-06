# Expected Usage:
#   include the file after defining the following variables
#    - TARGET_EXEC  : The name of the executable to build (default: a.out)
#    - BUILD_DIR    : The directory to put build files (default: build)
#    - SOURCE_DIRS  : The directory of the source files to build (.cpp, .c, .etc) (default: src)
#    - INCLUDE_DIRS : If header files are in a seperate directory, this would be where you would specify that (default: <empty>)
#    - CXXFLAGS     : Extra flags to pass to both C++ and C compilers
#    - CFLAGS       : Extra flags to pass to just the C compiler
#    - CPPFLAGS     : Extra flags to pass to just the C++ compiler
#    - LDFLAGS      : Extra flags to pass to the linker

TARGET_EXEC  := $(or $(TARGET_EXEC),a.out)
BUILD_DIR    := $(or $(BUILD_DIR),build)
SOURCE_DIRS  := $(or $(SOURCE_DIRS),src)
INCLUDE_DIRS := $(or $(INCLUDE_DIRS),)

INC_DIRS := $(shell find $(SOURCE_DIRS) $(INCLUDE_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXXFLAGS := $(INC_FLAGS) $(CXXFLAGS)
CPPFLAGS := $(CXXFLAGS) -std=c++17
CFLAGS   := $(CFLAGS) -std=c17

DEBUG_FLAGS   := -g3 -Og
RELEASE_FLAGS := -Wall -Wextra -O3 $(if $(HARSH_OPTIMIZATION),-Werror -Wshadow -Wformat=2 -fno-common)

SRCS := $(shell find $(SOURCE_DIRS) -name *.cpp -or -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

C_COMPILER   ?= clang
CPP_COMPILER ?= clang++

$(BUILD_DIR)/$(TARGET_EXEC): CXXFLAGS+=$(if $(RELEASE),$(RELEASE_FLAGS),$(DEBUG_FLAGS))
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CPP_COMPILER) $< -o $@ $(LDFLAGS)

# C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CPP_COMPILER) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(C_COMPILER) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

.PHONY: all

all:
	$(info $$SRCS is [$(SRCS)])
	$(info $$OBJS is [$(OBJS)])
	@echo Done!

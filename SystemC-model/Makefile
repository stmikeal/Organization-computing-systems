INCLUDE_PATH=./include
SOURCE_PATH=./src
BUILD_PATH=./build
TARGET=main

SYSTEMC_INC_DIR  ?= $(SYSTEMC_HOME)/include
SYSTEMC_LIB_DIR  ?= $(SYSTEMC_HOME)/lib-linux64

CC=g++
INCLUDE_FLAGS = -I$(INCLUDE_PATH) -I$(SYSTEMC_INC_DIR)
LIB_FLAGS = -L$(SYSTEMC_LIB_DIR) -lsystemc -Wl,-rpath=$(SYSTEMC_LIB_DIR)
CFLAGS = -O3 -std=c++11 -DSC_DEFAULT_WRITER_POLICY=SC_MANY_WRITERS

SRCS:=$(shell find $(SOURCE_PATH) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_PATH)/%.o)

.PHONY: clean all build

all: clean build

build: $(TARGET) Makefile

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) $(LIB_FLAGS) -o $@

$(BUILD_PATH)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean:
	rm -rf build/ main
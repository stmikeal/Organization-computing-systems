INCLUDE_PATH=./include
SOURCE_PATH=./src
BUILD_PATH=./build
TARGET=main

CC=g++
CFLAGS= -O3 -std=c++17 -I$(INCLUDE_PATH) -B$(SOURCE_PATH)

SRCS:=$(shell find $(SOURCE_PATH) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_PATH)/%.o)

.PHONY: clean all build

all: clean build

build: $(TARGET) Makefile

$(TARGET): $(OBJS)
	echo $(patsubst ,.cpp,.o,$(SRCS))
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.cpp.o: %.cpp
	echo $@
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build/ main
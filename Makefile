INCLUDE_PATH=./include/
SOURCE_PATH=./src/
TEST_PATH=./sample/output/
TARGET=main

CC=g++
CFLAGS= -O3 -std=c++17 -I$(INCLUDE_PATH) -B$(SOURCE_PATH)

SRCS:=$(shell find $(SOURCE_PATH) -name '*.cpp' -or -name '*.c' -or -name '*.s')
TESTS_SRCS:=$(shell find $(TEST_PATH) -name '*.cpp' -or -name '*.c' -or -name '*.s')
TESTS:=$(patsubst %.cpp, %, $(TESTS_SRCS))

.PHONY: clean all build

all: clean build

$(TARGET): $(SRCS)
        mkdir TEST_PATH
        $(CC) $(CFLAGS) -o $@ $<

build: $(TARGET) Makefile

$(TESTS): $(TESTS_SRCS)
        $(CC) $(CFLAGS) -o $@ $@.cpp
        ./$@

tests: $(TESTS)

clean:
        rm -rf sample/output/
        rm -rf build/ main
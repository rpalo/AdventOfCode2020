CC=gcc
CFLAGS=-Wall -Iunity/src -Isrc
CLEAN=rm -rf
DAY:=$(shell tail -n1 current_day.txt)

.PHONY: run
run: bin/aoc
	@bin/aoc

.PHONY: all
all: clean test run

.PHONY: clean
clean:
	$(CLEAN) build/objs/*
	$(CLEAN) build/Test*.out

# Build main binary
bin/aoc: build/objs/main.o build/objs/Day$(DAY).o build/objs/parsing.o build/objs/hashmap.o
	@$(CC) $^ -o $@ $(CFLAGS)

# Build object files for main binary
build/objs/%.o: src/%.c
	@$(CC) -c $^ -o $@ $(CFLAGS)

# Build test executables
build/Test%.out: test/Test%.c src/%.c src/parsing.c src/hashmap.c unity/src/unity.c
	@$(CC) $^ -o $@ $(CFLAGS)

# Run all tests
TESTS=$(wildcard test/*.c)
TESTEXECS=$(patsubst test/Test%.c, build/Test%.out, $(TESTS))
.PHONY: test
test: $(TESTEXECS)
	@$(foreach ex, $(TESTEXECS), echo "Running $(ex)"; ./$(ex);)

.PHONY: testone
testone: build/TestDay$(DAY).out
	./$<
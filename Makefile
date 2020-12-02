CC=gcc
CFLAGS=-Wall -Werror -Iunity/src -Isrc

.PHONY: run
run: bin/aoc
	@bin/aoc

# Build main binary
bin/aoc: build/objs/main.o build/objs/Day2.o build/objs/parsing.o
	@$(CC) $^ -o $@ $(CFLAGS)

# Build object files for main binary
build/objs/%.o: src/%.c
	@$(CC) -c $^ -o $@ $(CFLAGS)

# Build test executables
build/Test%.out: test/Test%.c src/%.c src/parsing.c unity/src/unity.c
	@$(CC) $^ -o $@ $(CFLAGS)

# Run all tests
TESTS=$(wildcard test/*.c)
TESTEXECS=$(patsubst test/Test%.c, build/Test%.out, $(TESTS))
.PHONY: test
test: $(TESTEXECS)
	@$(foreach ex, $(TESTEXECS), echo "Running $(ex)"; ./$(ex);)
# Shared Makefile included by every sys/*/c/Makefile.
COMMON := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# Prefer clang: Apple clang ships ASan/UBSan runtimes; some gcc builds (e.g. nix) don't.
ifeq ($(origin CC),default)
CC      := $(shell command -v clang 2>/dev/null || echo cc)
endif
SAN     ?= -fsanitize=address,undefined
CFLAGS  ?= -std=c17 -Wall -Wextra -Wpedantic -Wshadow -g -O1 -fno-omit-frame-pointer $(SAN)
CPPFLAGS += -D_DEFAULT_SOURCE -D_DARWIN_C_SOURCE -I$(COMMON) -include $(COMMON)/track_alloc.h
# per-OS build dir, so the docker toolbox never runs a macOS binary
BUILD   := build/$(shell uname -s)
LDLIBS  += -lm
SRCS    := $(wildcard *.c) $(COMMON)/track_alloc.c

.PHONY: test valgrind clean
test: $(BUILD)/test
	@./$(BUILD)/test

$(BUILD)/test: $(SRCS) $(wildcard *.h) $(COMMON)/test.h $(COMMON)/track_alloc.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SRCS) -o $@ $(LDLIBS)

# Same tests without sanitizers, under valgrind (Linux / docker toolbox: lc box valgrind <id>).
valgrind: $(SRCS) $(wildcard *.h)
	@mkdir -p $(BUILD)
	$(CC) $(filter-out $(SAN),$(CFLAGS)) $(CPPFLAGS) $(SRCS) -o $(BUILD)/test-vg $(LDLIBS)
	LC_NOFORK=1 valgrind -q --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(BUILD)/test-vg

clean:
	rm -rf $(BUILD)

#We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

C_COMPILER=gcc

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wundef

TEST_RUNNERS_DIR=test/test_runners
INC_DIRS=-Isrc -I/src

main: clean
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) src/main.c src/alloc.c src/hashtable.c -o main

run: main
	./main

clean:
	$(CLEANUP) rm -f *.exe *.msi *.so *.o main $(TEST_RUNNERS_DIR)/*

TEST_1 = test1$(TARGET_EXTENSION)
SRC_FILES_1=vendor/unity/unity.c test/hashtable_test.c $(TEST_RUNNERS_DIR)/hashtable_test_Runner.c

test: clean $(SRC_FILES_1)
	$(C_COMPILER) $(CFLAGS) -DMODE_TEST $(INC_DIRS) $(SRC_FILES_1) -o $(TEST_1)
	- ./$(TEST_1)

test/test_runners/hashtable_test_Runner.c: test/hashtable_test.c
	ruby scripts/generate_test_runner.rb test/hashtable_test.c  $(TEST_RUNNERS_DIR)/hashtable_test_Runner.c

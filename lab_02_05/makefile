CC := gcc

BUILDFLAGS := -lm
CFLAGS := -std=c99 -Wall -Werror -Wextra -pedantic
DEBUGFLAGS := -Og -g3 -fprofile-arcs -ftest-coverage -DDEBUG

# release build part

.PHONY: release
release: CFLAGS += -O3 -g0
release: release.lastbuildstate | app.exe

# files

app.exe: main.o record_table.o utils.o record.o
	$(CC) $(BUILDFLAGS) $^ -o $@

unit_tests.exe: unit_tests.o record_table.o record.o
	$(CC) $(BUILDFLAGS) $^ -o $@

table_generator.exe: table_generator.o utils.o record_table.o record.o
	$(CC) $(BUILDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.d : %.c
	$(CC) $(CFLAGS) -MM $< > $@

%.lastbuildstate: 
	make clean
	touch $@

# debug builds part

.PHONY: debug

debug: CFLAGS += $(DEBUGFLAGS)
debug: BUILDFLAGS += -fprofile-arcs -lgcov -ftest-coverage

ifeq ($(san), asan)
debug: CC:=clang
debug: CFLAGS += -fsanitize=address -fno-omit-frame-pointer
debug: BUILDFLAGS += -fsanitize=address
debug: asan.lastbuildstate | app.exe

else ifeq ($(san), msan)
debug: CC:=clang
debug: CFLAGS += -fsanitize=memory -fPIE -fno-omit-frame-pointer
debug: BUILDFLAGS += -fsanitize=memory -fPIE
debug: msan.lastbuildstate | app.exe

else ifeq ($(san), ubsan)
debug: CC:=clang
debug: CFLAGS += -fsanitize=undefined -fno-omit-frame-pointer
debug: BUILDFLAGS += -fsanitize=undefined
debug: ubsan.lastbuildstate | app.exe

else
debug: debug.lastbuildstate | app.exe

endif

# test build part

.PHONY: test_build
test_build: CFLAGS += $(DEBUGFLAGS)
test_build: BUILDFLAGS += -fprofile-arcs -lgcov
test_build: debug.lastbuildstate | unit_tests.exe

# utils part

.PHONY: weverything
weverything: CC:=clang
weverything: CFLAGS += $(DEBUGFLAGS) -Weverything
weverything: BUILDFLAGS += -fprofile-arcs -lgcov
weverything: weverything.lastbuildstate | app.exe

.PHONY: generator
generator: release.lastbuildstate | table_generator.exe

.PHONY: clean
clean:
	rm -f ./*.exe ./*.o ./*.gcov ./*.gcno ./*.gcda out.txt ./*.d ./*.lastbuildstate

.PHONY: coverage
coverage:
	gcov ./*.gcno

.PHONY: unit
unit:
	make clean
	make test_build
	./unit_tests.exe
	make coverage

.PHONY: test
test:
	make clean
	make debug
	./func_tests/scripts/func_tests.sh
	make coverage

SRCS := $(wildcard *.c)
include $(SRCS:.c=.d)


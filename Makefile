LUA ?= lua
CFLAGS ?= -D_POSIX_C_SOURCE=200809L -fPIC -O3 -Ideps -Wall -Werror -Wextra -Wpedantic -std=c11

LIBFLAG = -shared

TARGET = sock99.so
TESTSRC = test/sock99.lua

# all the source files
SRC = $(wildcard src/*.c)
SRC += $(wildcard deps/*/*.c)

OBJS = $(SRC:.c=.o)

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(DEP_FLAG) $(CFLAGS) $(LDFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $(LIBFLAG) $(OBJS)

.PHONY: test
test:
	$(LUA) $(TESTSRC)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

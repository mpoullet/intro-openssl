# Using implicit rules as much as possible

CPPFLAGS += -Wall -Wextra -pedantic -g
CFLAGS += -std=c11
LDLIBS += -lssl -lcrypto

withssl: withssl.o
nossl: nossl.o

.PHONY: all
all: withssl nossl

.PHONY: clean
clean:
	@rm -rf *.o
	@rm -rf nossl
	@rm -rf withssl

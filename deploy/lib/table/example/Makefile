.POSIX:
LINK = $(CC)
CFLAGS = -g -Og
M_LDFLAGS = -L$(CHPREFIX)/lib -ltable
M_CFLAGS = -I$(CHPREFIX)/include

M = \
int\
stream\

all: $M
.o:; $(LINK) $< $(LDFLAGS) $(M_LDFLAGS) -o $@
.c.o:; $(CC) $(CFLAGS) $(M_CFLAGS) $< -c

.c:
%: %.c
.SUFFIXES: .o .c

lint:; make CFLAGS='-Wall -Wextra -g -O2'
.PHONY: clean lint
clean:; rm -f -- $M $(M:=.o)

TARGET?=

BIN=supercop

VERSION?=n/a

SRC:=
SRC+=$(wildcard src/*.c)
SRC+=$(wildcard src/*/*.c)
SRC+=$(wildcard src/*/*/*.c)
SRC+=$(wildcard src/*/*/*/*.c)

# Ourselves
MTUNE?=native
MARCH?=native
CFLAGS?=
CFLAGS+=-D VERSION=\"$(VERSION)\"
CFLAGS+=-mtune=$(MTUNE) -march=$(MARCH)
CFLAGS+=-O2 -pipe
CFLAGS+=-Wall -Wextra

INCLUDES:=
INCLUDES+=-I src

include lib/.dep/config.mk

# Target-specific overrides, optional.
# Supplied by target/<target>/Makefile.target
-include Makefile.target

CFLAGS+=$(INCLUDES)

OBJ:=$(SRC:.c=.o)

.PHONY: default
default: $(BIN)

%.o: %.c $(LIBS)
	$(CC) $(CFLAGS) $(@:.o=.c) -D__NAME=\"$(BIN)\" -D__TARGET=\"$(TARGET)\" -c -o $@

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.PHONY: static
static:
	CFLAGS="-static -Os -s" $(MAKE) "MTUNE=$(MTUNE)" "MARCH=$(MARCH)" $(BIN)
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $(BIN)

.PHONY: clean
clean:
	rm -f $(OBJ) $(BIN)

.PHONY: install
install: $(BIN)
	install -Dm0755 $(BIN) $(DESTDIR)/bin/$(BIN)
	install -Dm0644 man/supercop.1 $(DESTDIR)/share/man/man1/supercop.1

include config.mk

SRC:=
SRC+=$(wildcard src/*.c)
SRC+=$(wildcard src/*/*.c)
SRC+=$(wildcard src/*/*/*.c)

# Ourselves
MTUNE?=native
MARCH?=native
CFLAGS?=
CFLAGS+=-D VERSION=\"$(VERSION)\"
CFLAGS+=-mtune=$(MTUNE) -march=$(MARCH)
CFLAGS+=-O2 -pipe
CFLAGS+=-Wall

INCLUDES?=
INCLUDES+=-Isrc

include lib/.dep/config.mk

# Map SRC into OBJ
OBJ:=$(patsubst %.c,%.o,$(SRC))

.PHONY: default
default: supercop

README.md:
	man --html=cat ./man/supercop.1 > README.md

supercop: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ)

.PHONY: small
static:
	CFLAGS="-static -Os -s" $(MAKE) "MTUNE=$(MTUNE)" "MARCH=$(MARCH)" supercop
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag supercop

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: install
install: supercop
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 supercop ${DESTDIR}${PREFIX}/bin
	install -d ${DESTDIR}${MANPREFIX}/man1
	install -m 644 $(wildcard man/*.1) ${DESTDIR}${MANPREFIX}/man1

.PHONY: clean
clean:
	rm -rf $(OBJ)

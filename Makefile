include config.mk

# Ourselves
CFLAGS?=
CFLAGS+=-D VERSION=\"$(VERSION)\"
CFLAGS+=-O2 -mtune=native -march=native -pipe
CFLAGS+=-Wall
INCLUDES?=
INCLUDES+=-Isrc
SRC:=
SRC+=$(wildcard src/*.c)
SRC+=$(wildcard src/*/*.c)
SRC+=$(wildcard src/*/*/*.c)

# lib/argparse
INCLUDES+=-Ilib/argparse
SRC+=lib/argparse/argparse.c

# lib/ed25519
INCLUDES+=-Ilib/ed25519/src
# SRC+=lib/ed25519/src/add_scalar.c
SRC+=lib/ed25519/src/fe.c
SRC+=lib/ed25519/src/ge.c
SRC+=lib/ed25519/src/key_exchange.c
SRC+=lib/ed25519/src/keypair.c
SRC+=lib/ed25519/src/sc.c
SRC+=lib/ed25519/src/seed.c
SRC+=lib/ed25519/src/sha512.c
SRC+=lib/ed25519/src/sign.c
SRC+=lib/ed25519/src/verify.c

# Map SRC into OBJ
OBJ:=$(patsubst %.c,%.o,$(SRC))

.PHONY: default
default: supercop

README.md:
	man --html=cat ./man/supercop.1 > README.md

supercop: lib/argparse lib/ed25519 $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ)

.PHONY: small
small:
	$(MAKE) "CFLAGS=-static -Os -s -fno-stack-protector -fomit-frame-pointer -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -ffast-math -Wl,-z,norelro -Wl,--hash-style=gnu -Wl,--build-id=none" supercop
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag supercop

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

lib/argparse:
	mkdir -p lib/argparse
	curl -sL https://github.com/cofyc/argparse/archive/refs/heads/master.tar.gz | tar xzv -C lib/argparse --strip-components=1

lib/ed25519:
	mkdir -p lib/ed25519
	curl -sL https://github.com/orlp/ed25519/archive/refs/heads/master.tar.gz | tar xzv -C lib/ed25519 --strip-components=1
	bash -c 'cd lib/ed25519 && patch -p1 < ../../patch/ed25519/00-single-file-compile.patch'

.PHONY: install
install: supercop
	install -d ${DESTDIR}${PREFIX}/bin
	install -m 755 supercop ${DESTDIR}${PREFIX}/bin
	install -d ${DESTDIR}${MANPREFIX}/man1
	install -m 644 $(wildcard man/*.1) ${DESTDIR}${MANPREFIX}/man1

.PHONY: clean
clean:
	rm -rf $(OBJ)

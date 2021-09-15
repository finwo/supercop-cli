# Ourselves
CFLAGS?=
CFLAGS+=-O2 -mtune=native -march=native -pipe -Wall
INCLUDES?=
INCLUDES+=-Isrc
SRC:=$(wildcard src/*.c)

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

supercop: lib/argparse lib/ed25519 $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

lib/argparse:
	mkdir -p lib/argparse
	curl -sL https://github.com/cofyc/argparse/archive/refs/heads/master.tar.gz | tar xzv -C lib/argparse --strip-components=1

lib/ed25519:
	mkdir -p lib/ed25519
	curl -sL https://github.com/orlp/ed25519/archive/refs/heads/master.tar.gz | tar xzv -C lib/ed25519 --strip-components=1
	bash -c 'cd lib/ed25519 && patch -p1 < ../../patch/ed25519/00-single-file-compile.patch'

.PHONY: clean
clean:
	rm -rf $(OBJ)
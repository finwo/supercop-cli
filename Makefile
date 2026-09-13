TARGET:=linux-glibc-amd64

BIN:=supercop

VERSION?=v1.0.0

.PHONY: default
default: build/${TARGET}/${BIN}

FIND:=$(shell command -v gfind find)

WATCH?=
WATCH+=$(shell $(FIND) src -type f -name '*.c')
WATCH+=$(shell $(FIND) src -type f -name '*.h')

build/${TARGET}/${BIN}: build/${TARGET} $(WATCH)
	cd build/${TARGET} && dep install
	$(MAKE) --directory build/${TARGET} TARGET=${TARGET} VERSION=${VERSION}

build/${TARGET}: $(WATCH)
	mkdir -p build/${TARGET}
	cp -rT src/              build/${TARGET}/src
	cp -rT man/              build/${TARGET}/man
	cp -rT target/common/    build/${TARGET}
	cp -rT target/${TARGET}/ build/${TARGET}

.PHONY: targets
targets:
	@ls -1 target | grep -v '^common$$'

README.md: ./man/supercop.1
	pandoc $< --output=$@

.PHONY: clean
clean:
	rm -rf build

# Never let the catch-all try to rebuild the makefiles themselves
Makefile: ;

# Forward any other goal verbatim into the assembled target tree
.PHONY: FORCE
FORCE:
%: build/${TARGET} FORCE
	cd build/${TARGET} && dep install
	$(MAKE) --directory build/${TARGET} TARGET=${TARGET} VERSION=${VERSION} $@

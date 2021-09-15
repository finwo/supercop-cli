# Key encodings

Keys can (or will) be encoded in multiple formats. This file shows a list of
encodings supported by this program.

The first byte of a key-file must ALWAYS start with the version identifier
(byte sequence prefix), allowing a different prefix to identify a different type
of key encoding.

## 0x00

Plain, binary, ed25519.

Not text-safe. Starts with a 0x00 byte, followed by 32 bytes of the raw public
key, then followed by 64 bytes of the raw private key.

Intended as the first format supported and to be deprecated later. Meant to be
simple to code, not to be safe or flexible.

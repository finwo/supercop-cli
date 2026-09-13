# Key encodings

Keys can (or will) be encoded in multiple formats. This file shows a list of
encodings supported by this program.

Each key-file must start with a distinct prefix (byte sequence), allowing a
different prefix to identify a different type of key encoding.

A decoded key is either full (public + private half) or public-only. A
`NULL` private half means public-only; commands requiring the private half
(`sign`) refuse such keys while `verify` accepts them. A lone private half
additionally yields its public half via scalar multiplication.

## 0x00

Plain, binary, ed25519.

Not text-safe. Starts with a 0x00 byte, followed by 32 bytes of the raw public
key. Full keys additionally carry 64 bytes of the raw private key after that,
so valid sizes are 33 (public-only) or 97 (full) bytes.

Intended as the first format supported and to be deprecated later. Meant to be
simple to code, not to be safe or flexible.

## asc

Ascii-armored raw key bytes, no prefix. A full key emits both a PRIVATE
block (64 private bytes) and a PUBLIC block (32 public bytes):

```
-----BEGIN SUPERCOP PRIVATE KEY-----
<base64, 64 columns>
-----END SUPERCOP PRIVATE KEY-----
-----BEGIN SUPERCOP PUBLIC KEY-----
<base64, 64 columns>
-----END SUPERCOP PUBLIC KEY-----
```

Public-only keys emit just the PUBLIC block. Labels are namespaced so
openssl never mistakes the raw payload for DER/PKCS#8. When both blocks
are present, the private block is authoritative and the public block must
agree with the derived public half; a corrupt private block fails closed.

## hdr

General-purpose text-protocol header encoding. Byte-identical to what
`printkey` outputs:

```
public-key: <64 hex>
private-key: <128 hex | (no private key)>
```

Parsing rules, meant to make integration in other programs easy:

- Key lines may appear on any line, in any order, surrounded by any other
  content; unknown labels are ignored.
- Labels are case-insensitive, with 0 or more spaces/tabs around the `:`.
- A parenthesized value (e.g. `(no private key)`) marks the field absent; a
  missing `private-key:` line means the same. First valid occurrence wins,
  malformed hex runs are skipped.
- A lone `private-key:` line derives its public half; a file with neither
  key line is invalid.

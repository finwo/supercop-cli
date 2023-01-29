# NAME

supercop - Simplified CLI for supercop-ref10

# SYNOPSIS

**supercop** *\<operation\>* \[**-k** *keyfile*\] \[**-m** *message*\]
\[**-M** *message-file*\] \[**-s** *signature*\]

# DESCRIPTION

This program allows easy signing and signature verification for the
supercop-ref10 implementation of the ed25519 signature scheme.

# OPERATIONS

**\--printkey**

:   Print contents of key file

**\--generate**

:   Generate a new key

**\--sign**

:   Sign a message

**\--verify**

:   Verify a message signature

**\--version**

:   Show version number and exit

# OPTIONS

**-h, \--help**

:   Show usage and exit

**-k, \--key-file *path***

:   Select key file to use for the operation

**-m, \--message *message_string***

:   Message to sign or verify (defaults to stdin)

**-M, \--message-file *path***

:   Message file to sign or verify (defaults to stdin)

**-s, \--signature *signature_string***

:   Signature to verify the message against

# RETURN VALUE

Here are the possible return values:

> -   **0 Everything went fine or the signature was valid**
>
> -   **1 Signature was invalid or an error occurred, see stderr for an
>     explanation**

# ENVIRONMENT

supercop doesn\'t follow any environment variables

# AUTHOR

finwo (https://finwo.nl)

# NAME

supercop - Simplified CLI for supercop-ref10

# SYNOPSIS

**supercop** \[*global options*\] *command* \[*command options*\]\
**supercop** **list-commands**\
**supercop** **\--version**

# DESCRIPTION

This program allows easy signing and signature verification for the
supercop-ref10 implementation of the ed25519 signature scheme.

Global options must appear before the command name. Run **supercop**
*command* **\--help** for the details on a single command.

# GLOBAL OPTIONS

**-h, \--help**

:   Show usage and exit

**-V, \--version**

:   Show version number and exit (same as **supercop version**)

# COMMANDS

**generate \[**-k** *keyfile*\]**

:   Generate a new key, writing to keyfile or stdout

**printkey **-k** *keyfile***

:   Print contents of key file

**sign **-k** *keyfile* \[**-m** *message* \| **-M** *message-file*\]**

:   Sign a message

**verify **-k** *keyfile* **-s** *signature* \[**-m** *message* \| **-M** *message-file*\]**

:   Verify a message signature

**version**

:   Show version number and exit (same as **supercop \--version**)

**list-commands**

:   List available commands

# COMMAND OPTIONS

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

# FILES

supercop uses no configuration files

# AUTHOR

finwo (https://finwo.nl)

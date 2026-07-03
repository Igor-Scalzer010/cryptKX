# Security Notes

## Inadequate generators

`rand()` and `std::mt19937` must not be used for passwords. They are designed for simulations, games, tests, or reproducible random draws, not for secrets. Even when their output looks random, their internal state can be predicted or reproduced with enough information.

## CSPRNG

Passwords require a CSPRNG (*Cryptographically Secure Pseudorandom Number Generator*), a cryptographically secure pseudorandom number generator. In cryptKX, libsodium is responsible for that. The `randombytes_*` family uses secure operating-system sources and is appropriate for generating character indexes.

To choose a character from an alphabet, prefer `randombytes_uniform(alphabetSize)`. This avoids the modulo bias introduced when a large random number is reduced with `% alphabetSize`.

## Clipboard

When the user copies a password, it moves to the operating-system clipboard. Other applications or clipboard history tools may access that content depending on the machine configuration. Treat the clipboard as a convenience, not as secure storage.

## No history

The application must not keep generated-password history or write passwords to disk, logs, temporary files, or telemetry. The current password lives only in application memory and, when copied, in the system clipboard.

## Core rules

Some design choices in the core are applied to maintain password-generation security:
- Allowed length: 8 to 64 characters;
- Default value: 20 characters;
- At least one character group must be selected;
- Generation uses `randombytes_uniform()` for each character index.

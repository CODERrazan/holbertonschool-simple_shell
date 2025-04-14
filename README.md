# Simple Shell

## Description
The Simple Shell project is an implementation of a basic command-line interpreter similar to `/bin/sh`. This shell reads user input, parses commands, executes programs, and handles errors.

## Features
- Execute programs using absolute or relative paths (e.g., `/bin/ls`).
- Support for interactive and non-interactive modes.
- Parses and uses the `PATH` environment variable.
- Handles errors gracefully and ensures proper memory management.
- Suspends processes until child processes terminate.

## Requirements
- Compiled on **Ubuntu 20.04 LTS** with **GCC**.
- Follows **Betty coding style** guidelines.
- No memory leaks in any functionality.
- Header files are include-guarded.

## How to Compile
To compile the shell, use the following command:
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

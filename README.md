# Linux C Shell

A simple implementation of a linux shell written in C language.

## Features

- The 'cd' command.
- Running background processes by typing 'fg' or Ctrl + Z.
- Tab filename expansion.
- Pipe as well as multiple pipe commands.
- By typing the word 'exit' the shell exits.
- Redirection of input and output (<,>,>>).
- Any default linux command that can be run by the 'execvp' instruction.

## Compiling and running
The shell uses the readline header file which must be installed in order to run the program:
```bash
sudo apt-get install libreadline-dev
```
 A makefile is provided in the repo and can be run with the command:
```bash
make shell
```


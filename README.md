# Simple Shell
A simple shell written in C. Multiple commands may be separated by ';' or '|', but input and output redirection is not implemented. Basic functionality such as "cd" and "pwd" are provided. Any other valid input will be launched using the fork() system call. Uses the GNU readline library for user input.

# Sample output
```
[dan@server simpleshell]$ ./simpleshell
> pwd
/home/dan/simpleshell
> ;ls -l |       whoami ;;
Pipe not implemented
total 32
-rw-r--r-- 1 dan dan   114 Feb 21 04:55 Makefile
-rw-r--r-- 1 dan dan  3802 Feb 21 15:27 simpleshell.c
-rwxr-xr-x 1 dan dan 13648 Feb 21 15:28 simpleshell
-rw-r--r-- 1 dan dan  3434 Feb 21 15:27 tokenizer.c
-rw-r--r-- 1 dan dan   755 Feb 21 15:27 tokenizer.h
Process exited successfully
dan
Process exited successfully
> ls; cat Makefile > out.txt; whoami;
Makefile  simpleshell.c  simpleshell  tokenizer.c  tokenizer.h
Process exited successfully
IO redirection and background not implemented
CC = gcc
CFLAGS = -std=c99 -lreadline

simpleshell: simpleshell.c tokenizer.c
        $(CC) $(CFLAGS) -o simpleshell simpleshell.c tokenizer.c
Process exited successfully
dan
Process exited successfully
> cd doesnt_exist
Directory does not exist or is not accessible.
> cd too many args
Accepts exactly one argument
> cd ..| cd ..
Pipe not implemented
> pwd
/home
> doesnt exist 1 2 3
Unable to execute doesnt
> whoami red
whoami: extra operand 'red'
Try 'whoami --help' for more information.
Process exited with error
> echo hello world &
IO redirection and background not implemented
hello world
Process exited successfully
>
```
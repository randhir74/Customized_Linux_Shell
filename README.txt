I present to you the Justy-shell. This is a shell program written in c using
execvp library and forking.

//Help
The instructions it supports:
  1. pwd
  2. cd dir
  3. mkdir
  4. rmdir
  5. exit
  6. a.out | b.out
  7. c.out < input.txt > output.txt*
* - both the input and output need not be specified, it can take input from
terminal also and output to terminal also.

//Makefile
I have included a makefile which compiles all the .c files and runs the shell.
The commands makefile supports:
  1. make
  2. make clean

//Directory structure
                   Top
                    |
___________________________________
|                   |             |
>cs1180386_sh.c   makefile     inp.txt
>assignment2.c                 res.txt
>test1.c                       out.txt
>test2.c                       README.txt

//Brief description to the code
cs1180286_sh.c is a c program that executes the object file of assignment2.c
using forking and execvp functions. assignment2.c is the program which would run
execute the commands entered by the user in terminal. test1.c test2.c can be used
testing piping and input output redirections.

//Few Notes
1. Piping works only for two files i.e. instructions of the type a.out | b.out.
2. Piping and input, output indirection won't work together i.e. commands of
type a.out | b.out > c.txt won't be executed as expected on a bash terminal.
